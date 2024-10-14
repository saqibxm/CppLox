#include <sstream>

#include "Parser/Expression.hpp"
#include "Parser/Visitor.hpp"

using namespace lox;

std::string lox::ASTPrinter::print(const Expression &expr)
{
	return std::any_cast<std::string>(expr.accept(*this));
}

// exists in source file because its only used here, not from outside
template <typename...Args>
std::string lox::ASTPrinter::parenthesize(std::string name, const Expression& first, const Args&...rest)
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

std::string lox::ASTPrinter::add_space(const Expression &expr)
{
	return " " + std::any_cast<const std::string&>(expr.accept(*this));
}


std::any ASTPrinter::visit(const Binary& expr)
{
	return parenthesize(expr.operation.lexeme, *expr.left, *expr.right);
}
std::any ASTPrinter::visit(const Unary &expr)
{
	return parenthesize(expr.operation.lexeme, *expr.operand);
}
std::any lox::ASTPrinter::visit(const Conditional &expr)
{
	return parenthesize("conditional", *expr.condition, *expr.left, *expr.right);
}
std::any ASTPrinter::visit(const Grouping &expr)
{
	return parenthesize("group", *expr.expression);
}
std::any ASTPrinter::visit(const Operator &expr)
{
	return expr.operation.lexeme;
}
std::any ASTPrinter::visit(const Value &expr)
{
	return expr.value.empty() ? "nul" : expr.value.str();
}

/* vv XPrinter vv*/
/*
std::string lox::XPrinter::print(const Expression &expr) const
{
	return expr.accept(*this);
}

std::string lox::XPrinter::visit(const Binary & exp) const
{
	return parenthesize(exp.operation.lexeme, *exp.left, *exp.right);
}

std::string lox::XPrinter::visit(const Unary & exp) const
{
	return parenthesize(exp.operation.lexeme, *exp.operand);
}

std::string lox::XPrinter::visit(const Conditional & exp) const
{
	return parenthesize("conditional", *exp.condition, *exp.left, *exp.right);
}

std::string lox::XPrinter::visit(const Grouping & exp) const
{
	return parenthesize("group", *exp.expression);
}

std::string lox::XPrinter::visit(const Operator & exp) const
{
	return exp.operation.lexeme;
}

std::string lox::XPrinter::visit(const Value & exp) const
{
	return exp.value.empty() ? "nul" : exp.value.str();
}

std::string lox::XPrinter::add_space(const Expression &expr) const
{
	return " " + print(expr);
}

template<typename...Args>
std::string XPrinter::parenthesize(std::string name, const Expression &first, const Args &...rest) const
{
	static_assert((... && std::is_same_v<decltype(first), decltype(rest)>), "only Expression type is accepted!");

	std::ostringstream strm;
	strm << "(" << name;
	strm << add_space(first);
	(strm << ... << add_space(rest));
	strm << ")";

	return strm.str();
}
*/