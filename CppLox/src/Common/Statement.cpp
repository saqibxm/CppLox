#include "Common/Statement.hpp"

using namespace lox::stmt;
using namespace lox;

stmt::Expression::Expression(Expr &&expr)
	: expression(std::move(expr))
{
}

void lox::stmt::Expression::accept(Visitor &visitor)
{
	visitor.visit(*this);
}

Print::Print(Expr &&expr)
	: expression(std::move(expr))
{
}

void lox::stmt::Print::accept(Visitor &visitor)
{
	visitor.visit(*this);
}

lox::stmt::Var::Var(const Token &vname, Expr &&init)
	: name(vname), initializer(std::move(init))
{
}

void lox::stmt::Var::accept(Visitor &visitor)
{
	visitor.visit(*this);
}
