#include "Common/Statement.hpp"

using namespace lox::stmt;
using namespace lox;

stmt::Expression::Expression(Expr &&expr)
	: expression(std::move(expr))
{
}

void lox::stmt::Expression::accept(Visitor &visitor) const
{
	visitor.visit(*this);
}

Print::Print(Expr &&expr)
	: expression(std::move(expr))
{
}

void lox::stmt::Print::accept(Visitor &visitor) const
{
	visitor.visit(*this);
}

lox::stmt::Var::Var(const Token &vname, Expr &&init)
	: name(vname), initializer(std::move(init))
{
}

void lox::stmt::Var::accept(Visitor &visitor) const
{
	visitor.visit(*this);
}

lox::stmt::Block::Block(StatementList &&stmts)
	: statements(std::move(stmts))
{
}

void lox::stmt::Block::accept(Visitor &visitor) const
{
	visitor.visit(*this);
}
