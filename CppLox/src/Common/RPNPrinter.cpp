#include "Common/RPNPrinter.hpp"
#include "Common/Expression.hpp"

using namespace lox;
using namespace lox::expr;

std::string lox::RPNPrinter::print(const Expression &expr)
{
	return "RPN: " + std::any_cast<const std::string&>(expr.accept(*this));
}

std::any RPNPrinter::visit(const Binary& expr)
{
	return stringify(*expr.left) + ' ' + stringify(*expr.right) + ' ' + expr.operation.lexeme;
}
std::any RPNPrinter::visit(const Unary &expr)
{
	return '{' + stringify(*expr.operand) + '}' + ' ' + expr.operation.lexeme;
}
std::any RPNPrinter::visit(const Conditional &expr)
{
	return std::string("Conditonal");
}
std::any RPNPrinter::visit(const Grouping &expr)
{
	return stringify(*expr.expression);
}
std::any RPNPrinter::visit(const Operator &expr)
{
	return expr.operation.lexeme;
}
std::any RPNPrinter::visit(const Value &expr)
{
	return expr.value.empty() ? "nul" : expr.value.str();
}

std::string lox::RPNPrinter::stringify(const Expression &expr)
{
	return std::any_cast<const std::string&>(expr.accept(*this));
}
