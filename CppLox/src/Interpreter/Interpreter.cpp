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

lox::Literal lox::Interpreter::evaluate(const expr::Expression &expr)
{
	return std::any_cast<const Literal&>(expr.accept(*this));
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
		return Literal(*plhs - *prhs);
		break;
	case TokenType::PLUS:
	{
		try
		{
			return Literal{ std::get<std::string>(left) + std::get<std::string>(right) };
		}
		catch (const std::bad_variant_access&)
		{
			check_number_operands(expr.operation, left, right);
			if (!(plhs && prhs)) throw RuntimeError(expr.operation, "Operands must be numbers of strings.");
			return Literal{ (*plhs + *prhs) }; // assuming both of them are same
		}
	} break;

	case TokenType::ASTERISK:
		check_number_operands(expr.operation, left, right);
		return Literal{ std::multiplies<>{}(*plhs, *prhs) };
		break;

	case TokenType::SLASH:
		check_number_operands(expr.operation, left, right);
		return Literal{ std::divides<>{}(*plhs, *prhs) };
		break;

	case TokenType::LESS:
		check_number_operands(expr.operation, left, right);
		return Literal{ *plhs < *prhs };
		break;

	case TokenType::LESS_EQUAL:
		check_number_operands(expr.operation, left, right);
		return Literal{ *plhs <= *prhs };
		break;

	case TokenType::GREATER:
		check_number_operands(expr.operation, left, right);
		return Literal{ *plhs > *prhs };
		break;

	case TokenType::GREATER_EQUAL:
		check_number_operands(expr.operation, left, right);
		return Literal{ *plhs >= *prhs };
		break;

	case TokenType::COMPARE:
		return Literal{ are_equal(left, right) };
		break;

	case TokenType::NOT_EQUAL:
		return Literal{ !are_equal(left, right) };

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
		return Literal{ std::logical_not<>()(is_true(operand)) };

	case TokenType::MINUS:
		validate_number(operation, operand);
		return Literal{ std::negate<>()(std::get<Operand>(operand)) };

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
		// return Literal(std::get<Operand>(operand) + 1);
		return Literal{ net };

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
		// return Literal{ std::get<Operand>(operand) - 1 };
		return Literal{ net };

	default:
		UNREACHABLE();
	}
	// environment->assign(ptr->name, Literal{ ++std::get<Operand>(operand) });
	return Literal{}; // monostate/default - no value
}

std::any lox::Interpreter::visit(const expr::Conditional &expr)
{
	auto condition = evaluate(*expr.condition);
	
	if (is_true(condition)) return evaluate(*expr.left);
	else return evaluate(*expr.right);

	return Literal{"Not convertible to a boolean!"}; // temporary error signaling
}

std::any lox::Interpreter::visit(const expr::Grouping &expr)
{
	return evaluate(*expr.expression);
}

std::any lox::Interpreter::visit(const expr::Operator &)
{
	return Literal{"Unused visitor!"};
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

void lox::Interpreter::visit(const stmt::Expression &stmt)
{
	evaluate(*stmt.expression);
}

void lox::Interpreter::visit(const stmt::Print &stmt)
{
	Literal value = evaluate(*stmt.expression);
	std::cout << value.str() << std::endl;
}

void lox::Interpreter::visit(const stmt::Var &stmt)
{
	Literal init; // default init to null
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
		Environment::Ptr previous;
		Environment::Ptr& target;
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

bool lox::Interpreter::is_true(const Literal &literal) //const
{
	if (auto val = std::get_if<bool>(&literal)) return *val; // bool
	if (auto val = std::get_if<std::string>(&literal)) return val->length();
	if (auto val = std::get_if<Operand>(&literal)) return *val != 0;

	if (std::holds_alternative<std::nullptr_t>(literal)) return false;
	return false; // implicit if literal holds nothing, i.e. monostate
}

bool lox::Interpreter::are_equal(const Literal &lhs, const Literal &rhs) //const
{
	if (lhs.index() != rhs.index()) return false; // the types held are not same: should raise an error
	return equal_impl(lhs, rhs);
}

bool lox::Interpreter::equal_impl(const Literal &lhs, const Literal &rhs) //const
{
	return std::visit([](const auto &left, const auto &right) -> bool {
		using T = std::decay_t < std::remove_reference_t<decltype(left) >> ;
		// static_assert(false, "must be same types");

		// using T = std::decay_t<std::remove_reference_t<typename std::common_type<decltype(left), decltype(right)>::type>>;
		if constexpr (!std::is_same_v<decltype(left), decltype(right)>)
			return false;
		else if constexpr (std::is_same<T, std::nullptr_t>::value || std::is_same<T, std::monostate>::value) return true;
		else return std::equal_to<T>{}(left, right);
	}, static_cast<Literal::Base>(lhs), static_cast<Literal::Base>(rhs));
}

void lox::Interpreter::validate_number(const Token &token, const Literal &operand)
{
	if (std::holds_alternative<Operand>(operand)) return;
	throw lox::RuntimeError(token, "Operand must be a number.");
}

void lox::Interpreter::check_number_operands(const Token &oper, const Literal &left, const Literal &right)
{
	if(std::holds_alternative<Operand>(left) && std::holds_alternative<Operand>(right)) return;

	throw lox::RuntimeError(oper, "Operands must be numbers.");
}
