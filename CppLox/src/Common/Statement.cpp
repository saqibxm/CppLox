#include "Common/Statement.hpp"

using namespace lox::stmt;
using namespace lox;

stmt::Expression::Expression(Expr &&expr)
	: expression(std::move(expr))
{
}

void lox::stmt::Expression::accept(StmtVisitor &visitor)
{
	visitor.visit(*this);
}

Print::Print(Expr &&expr)
	: expression(std::move(expr))
{
}

void lox::stmt::Print::accept(StmtVisitor &visitor)
{
	visitor.visit(*this);
}

lox::stmt::Var::Var(const Token &vname, Expr &&init)
	: name(vname), initializer(std::move(init))
{
}

void lox::stmt::Var::accept(StmtVisitor &visitor)
{
	visitor.visit(*this);
}

lox::stmt::Block::Block(StatementList &&stmts)
	: statements(std::move(stmts))
{
}

void lox::stmt::Block::accept(StmtVisitor &visitor)
{
	visitor.visit(*this);
}

lox::stmt::IfControl::IfControl(Expr &&cond, Stmt &&ifbody, Stmt &&elsebody)
	: condition(std::move(cond)), then_stmt(std::move(ifbody)), else_stmt(std::move(elsebody))
{
}

void lox::stmt::IfControl::accept(StmtVisitor &visitor)
{
	visitor.visit(*this);
}

lox::stmt::While::While(Expr &&cond, Stmt && body)
	: condition(std::move(cond)), body(std::move(body))
{
}

void lox::stmt::While::accept(StmtVisitor &visitor)
{
	visitor.visit(*this);
}

lox::stmt::LoopControl::LoopControl(Type type)
	: control(type)
{
}

void lox::stmt::LoopControl::accept(StmtVisitor &visitor)
{
	visitor.visit(*this);
}

lox::stmt::Function::Function(const Token &fname, const TokenList &params, StatementList &&stmts)
	: name(fname), parameters(params), body(std::move(stmts))
{
}

void lox::stmt::Function::accept(StmtVisitor &visitor)
{
	visitor.visit(*this);
}
