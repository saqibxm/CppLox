#include <functional>
#include <type_traits>
#include <iostream>

#include "Interpreter/Interpreter.hpp"

void lox::Interpreter::Interpret(const StatementList &statements)
{
	try
	{
		for (const Stmt &statement : statements)
			execute(*statement);
	}
	catch (const lox::RuntimeError &error)
	{
		diagnostics.RuntimeError(error);
	}
}

lox::Object lox::Interpreter::evaluate(const expr::Expression &expr)
{
	return std::any_cast<const Object&>(expr.accept(*this));
}

std::any lox::Interpreter::visit(const expr::Binary &expr)
{
	auto left = evaluate(*expr.left);
	auto right = evaluate(*expr.right);

	auto plhs = std::get_if<Operand>(&left);
	auto prhs = std::get_if<Operand>(&right);
	switch (expr.operation.type)
	{
	case TokenType::MINUS:
		check_number_operands(expr.operation, left, right);
		return Object(*plhs - *prhs);
		break;
	case TokenType::PLUS:
	{
		try
		{
			return Object{ std::get<std::string>(left) + std::get<std::string>(right) };
		}
		catch (const std::bad_variant_access&)
		{
			check_number_operands(expr.operation, left, right);
			if (!(plhs && prhs)) throw RuntimeError(expr.operation, "Operands must be numbers of strings.");
			return Object{ (*plhs + *prhs) }; // assuming both of them are same
		}
	} break;

	case TokenType::ASTERISK:
		check_number_operands(expr.operation, left, right);
		return Object{ std::multiplies<>{}(*plhs, *prhs) };
		break;

	case TokenType::SLASH:
		check_number_operands(expr.operation, left, right);
		return Object{ std::divides<>{}(*plhs, *prhs) };
		break;

	case TokenType::LESS:
		check_number_operands(expr.operation, left, right);
		return Object{ *plhs < *prhs };
		break;

	case TokenType::LESS_EQUAL:
		check_number_operands(expr.operation, left, right);
		return Object{ *plhs <= *prhs };
		break;

	case TokenType::GREATER:
		check_number_operands(expr.operation, left, right);
		return Object{ *plhs > *prhs };
		break;

	case TokenType::GREATER_EQUAL:
		check_number_operands(expr.operation, left, right);
		return Object{ *plhs >= *prhs };
		break;

	case TokenType::COMPARE:
		return Object{ are_equal(left, right) };
		break;

	case TokenType::NOT_EQUAL:
		return Object{ !are_equal(left, right) };

	default:
		UNREACHABLE();
		break;
	}

	return {};
}

std::any lox::Interpreter::visit(const expr::Unary &expr)
{
	auto operand = evaluate(*expr.operand);
	auto operation = expr.operation;

	Operand net = 0.0;
	bool flag = false;

	switch (operation.type)
	{
	case TokenType::NOT:
		return Object{ std::logical_not<>()(is_true(operand)) };

	case TokenType::MINUS:
		validate_number(operation, operand);
		return Object{ std::negate<>()(std::get<Operand>(operand)) };

	case TokenType::DOUBLE_PLUS: // incomplete, temporary
		validate_number(operation, operand);
		if (!flag) net = std::get<Operand>(operand), flag = true;
		if (auto ptr = dynamic_cast<expr::Variable*>(expr.operand.get()))
		{
			environment->assign(ptr->name, ++net);
			flag = false;
		}
		else ++net;
		// else throw lox::RuntimeError(operation, "Operand to increment is not an L-value.");
		// return Object(std::get<Operand>(operand) + 1);
		return Object{ net };

	case TokenType::DOUBLE_MINUS:
		validate_number(operation, operand);
		if (!flag) net = std::get<Operand>(operand), flag = true;

		if (auto ptr = dynamic_cast<expr::Variable*>(expr.operand.get()))
		{
			environment->assign(ptr->name, --net);
			flag = false;
		}
		else --net;
		// else throw lox::RuntimeError(operation, "Operand to decrement is not an L-value.");
		// return Object{ std::get<Operand>(operand) - 1 };
		return Object{ net };

	default:
		UNREACHABLE();
	}
	// environment->assign(ptr->name, Object{ ++std::get<Operand>(operand) });
	return Object{}; // monostate/default - no value
}

std::any lox::Interpreter::visit(const expr::Conditional &expr)
{
	auto condition = evaluate(*expr.condition);
	
	if (is_true(condition)) return evaluate(*expr.left);
	else return evaluate(*expr.right);

	return Object{"Not convertible to a boolean!"}; // temporary error signaling
}

std::any lox::Interpreter::visit(const expr::Grouping &expr)
{
	return evaluate(*expr.expression);
}

std::any lox::Interpreter::visit(const expr::Operator &expr)
{
	// return expr.operation.type == TokenType::BREAK ? Object{ true } : Object{ false };
	return Object{"Not Implemented"};
}

std::any lox::Interpreter::visit(const expr::Value &expr)
{
	return expr.value;
}

std::any lox::Interpreter::visit(const expr::Variable &expr)
{
	return environment->retrieve(expr.name);
}

std::any lox::Interpreter::visit(const expr::Assign &expr)
{
	auto value = evaluate(*expr.value);
	environment->assign(expr.name, value);
	return value; // return the right side of the assignment
}

std::any lox::Interpreter::visit(const expr::Logical &exp)
{
	auto operation = exp.operation.type;
	auto left = evaluate(*exp.left);
	// auto right = evaluate(*exp.right); // dont evaluate both

	if (operation == TokenType::OR || operation == TokenType::DOUBLE_LINE)
	{
		if (is_true(left)) return left;
		// return Literal {is_true(right)};
	}
	else if (operation == TokenType::AND || operation == TokenType::DOUBLE_AMP)
	{
		if (!is_true(left)) return left;
	}
	else UNREACHABLE();
	return evaluate(*exp.right);
}

void lox::Interpreter::visit(const stmt::Expression &stmt)
{
	evaluate(*stmt.expression);
}

void lox::Interpreter::visit(const stmt::Print &stmt)
{
	Object value = evaluate(*stmt.expression);
	std::cout << value.str() << std::endl;
}

void lox::Interpreter::visit(const stmt::Var &stmt)
{
	Object init; // default init to null
	if (stmt.initializer != nullptr) init = evaluate(*stmt.initializer);

	environment->define(stmt.name.lexeme, init);
}

void lox::Interpreter::visit(const stmt::Block &stmt)
{
	Environment::Ptr scope = std::make_shared<Environment>(environment); // environment->shared_from_this()
	execute_block(stmt.statements, scope);
}

void lox::Interpreter::visit(const stmt::IfControl &stmt)
{
	auto condition = evaluate(*stmt.condition);

	if (is_true(condition)) execute(*stmt.then_stmt);
	else if(stmt.else_stmt != nullptr) execute(*stmt.else_stmt);
}

void lox::Interpreter::visit(const stmt::While &stmt)
{
	// auto condition = evaluate(*stmt.condition); // donot evaluate once, evalute on every iteration.
	while (is_true(evaluate(*stmt.condition)))
	{
		try {
			execute(*stmt.body);
		} catch(const lox::ContinueExcept&) {
			continue;
		} catch (const lox::BreakExcept&) {
			break;
		}
	}
}

void lox::Interpreter::visit(const stmt::LoopControl &stmt)
{
	if (stmt.control == stmt::LoopControl::BREAK)
		throw BreakExcept();
	else throw ContinueExcept();
}

void lox::Interpreter::execute(const stmt::Statement &stmt)
{
	stmt.accept(*this);
}

void lox::Interpreter::execute_block(const StatementList & list, Environment::Ptr env)
{
	struct ScopedAssignment
	{
		ScopedAssignment(Environment::Ptr &target, Environment::Ptr &replacement)
			: target(target), previous(target)
		{
			target = replacement;
		}
		~ScopedAssignment()
		{
			target = previous;
		}
		Environment::Ptr& target;
		Environment::Ptr previous;
	} current(environment, env); // replace interpreter environment with the block scope env, save copy of the original
	// and replace it with original at the end

	for (const Stmt &statement : list) execute(*statement);
	/*
	Environment::Ptr previous = this->environment;
	try {
		environment = env;
		for (const Stmt &statement : list) execute(*statement);
	}
	catch (const lox::RuntimeError&)
	{
	}
	this->environment = previous;
	*/
}

bool lox::Interpreter::is_true(const Object &literal) //const
{
	if (auto val = std::get_if<bool>(&literal)) return *val; // bool
	if (auto val = std::get_if<std::string>(&literal)) return val->length();
	if (auto val = std::get_if<Operand>(&literal)) return *val != 0;

	if (std::holds_alternative<std::nullptr_t>(literal)) return false;
	return false; // implicit if literal holds nothing, i.e. monostate
}

bool lox::Interpreter::are_equal(const Object &lhs, const Object &rhs) //const
{
	if (lhs.index() != rhs.index()) return false; // the types held are not same: should raise an error
	return equal_impl(lhs, rhs);
}

bool lox::Interpreter::equal_impl(const Object &lhs, const Object &rhs) //const
{
	return std::visit([](const auto &left, const auto &right) -> bool {
		using T = std::decay_t < std::remove_reference_t<decltype(left) >> ;
		// static_assert(false, "must be same types");

		// using T = std::decay_t<std::remove_reference_t<typename std::common_type<decltype(left), decltype(right)>::type>>;
		if constexpr (!std::is_same_v<decltype(left), decltype(right)>)
			return false;
		else if constexpr (std::is_same<T, std::nullptr_t>::value || std::is_same<T, std::monostate>::value) return true;
		else return std::equal_to<T>{}(left, right);
	}, static_cast<Object::Base>(lhs), static_cast<Object::Base>(rhs));
}

void lox::Interpreter::validate_number(const Token &token, const Object &operand)
{
	if (std::holds_alternative<Operand>(operand)) return;
	throw lox::RuntimeError(token, "Operand must be a number.");
}

void lox::Interpreter::check_number_operands(const Token &oper, const Object &left, const Object &right)
{
	if(std::holds_alternative<Operand>(left) && std::holds_alternative<Operand>(right)) return;

	throw lox::RuntimeError(oper, "Operands must be numbers.");
}