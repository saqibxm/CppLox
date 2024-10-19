#include "Common/Expression.hpp"

using namespace lox;

expr::Binary::Binary(Expr lhs, Token op, Expr rhs)
	: left(std::move(lhs)), right(std::move(rhs)), operation(std::move(op))
{
}

/*
std::any expr::Binary::accept(ExprVisitor<std::any> visitor) const
{
	return visitor.visit(*this);
}*/

std::any expr::Binary::accept(ExprVisitor & visitor) const
{
	return visitor.visit(*this);
}

expr::Unary::Unary(Expr exp, Token op)
	: operand(std::move(exp)), operation(op)
{
}

/*
std::any expr::Unary::accept(ExprVisitor<std::any> visitor) const
{
	return visitor.visit(*this);
}*/

std::any expr::Unary::accept(ExprVisitor & visitor) const
{
	return visitor.visit(*this);
}

expr::Grouping::Grouping(Expr exp)
	: expression(std::move(exp))
{
}

/*std::any expr::Grouping::accept(ExprVisitor<std::any> visitor) const
{
	return visitor.visit(*this);
}*/

std::any expr::Grouping::accept(ExprVisitor & visitor) const
{
	return visitor.visit(*this);
}

expr::Operator::Operator(Token op)
	: operation(op)
{
}

/*std::any expr::Operator::accept(ExprVisitor<std::any> visitor) const
{
	return visitor.visit(*this);
}*/

std::any expr::Operator::accept(ExprVisitor & visitor) const
{
	return visitor.visit(*this);
}

expr::Value::Value(Object val)
	: value(std::move(val))
{
}

/*std::any expr::Value::accept(ExprVisitor<std::any> visitor) const
{
	return visitor.visit(*this);
}*/

std::any expr::Value::accept(ExprVisitor & visitor) const
{
	return visitor.visit(*this);
}


expr::Conditional::Conditional(Expr cond, Expr lhs, Expr rhs)
	: condition(std::move(cond)), left(std::move(lhs)), right(std::move(rhs))
{
}

std::any expr::Conditional::accept(ExprVisitor & visitor) const
{
	return visitor.visit(*this);
}

lox::expr::Variable::Variable(const Token &vname)
	: name(vname)
{
}

std::any lox::expr::Variable::accept(ExprVisitor & visitor) const
{
	return visitor.visit(*this);
}

lox::expr::Assign::Assign(const Token & vname, Expr && val)
	: name(vname), value(std::move(val))
{
}

std::any lox::expr::Assign::accept(ExprVisitor & visitor) const
{
	return visitor.visit(*this);
}

lox::expr::Logical::Logical(Expr && lhs, const Token & oper, Expr && rhs)
	: operation(oper), left(std::move(lhs)), right(std::move(rhs))
{
}

std::any lox::expr::Logical::accept(ExprVisitor & visitor) const
{
	return visitor.visit(*this);
}
