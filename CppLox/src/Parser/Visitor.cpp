#include <sstream>

#include "Parser/Expression.hpp"
#include "Parser/Visitor.hpp"

using namespace lox;

std::string ASTPrinter::visit(const Binary &exp)
{
	return "Not implemented";
}

std::string lox::ASTPrinter::print(const Expression &expr)
{
	// return std::any_cast<std::string>(expr.accept(*this));
	return "Not implemented";
}


std::string lox::ExperimentalPrinter::print(const Expression &expr)
{
	return std::any_cast<std::string>(expr.accept(*this));
}


template <typename...Args>
std::string lox::ExperimentalPrinter::parenthesize(std::string name, const Expression& first, const Args&...rest)
{
	static_assert((... && std::is_same_v<decltype(first), decltype(rest)>), "only Expression type is accepted!");

	std::ostringstream strm;
	strm << "(" << name;
	// strm << std::any_cast<const std::string>(first.accept(*this));
	strm << add_space(first);
	(strm << ... << add_space(rest));
	strm << ")";

	return strm.str();
}

std::string lox::ExperimentalPrinter::add_space(const Expression &expr)
{
	return " " + std::any_cast<const std::string&>(expr.accept(*this));
}


std::any ExperimentalPrinter::visit(const Binary& expr)
{
	return parenthesize(expr.operation.lexeme, *expr.left, *expr.right);
}
std::any ExperimentalPrinter::visit(const Unary &expr)
{
	return parenthesize(expr.operation.lexeme, *expr.operand);
}
std::any ExperimentalPrinter::visit(const Grouping &expr)
{
	return parenthesize("group", *expr.expression);
}
std::any ExperimentalPrinter::visit(const Operator &expr)
{
	return expr.operation.lexeme;
}
std::any ExperimentalPrinter::visit(const Value &expr)
{
	return expr.value.empty() ? "nul" : expr.value.get();
}