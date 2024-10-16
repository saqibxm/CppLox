#include "Common/Statement.hpp"

using namespace lox::stmt;
using namespace lox;

stmt::Expression::Expression(Expr &&expr)
	: expression(std::move(expr))
{
}

std::any lox::stmt::Expression::accept(Visitor &visitor)
{
	return visitor.visit(*this);
}

Print::Print(Expr &&expr)
	: expression(std::move(expr))
{
}

std::any lox::stmt::Print::accept(Visitor &visitor)
{
	return visitor.visit(*this);
}
