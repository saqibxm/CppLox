#include "Parser/Expression.hpp"

using namespace lox;

lox::Binary::Binary(Expr lhs, Token op, Expr rhs)
	: left(std::move(lhs)), right(std::move(rhs)), operation(std::move(op))
{
}

/*
std::any lox::Binary::accept(Visitor<std::any> visitor) const
{
	return visitor.visit(*this);
}*/

std::any lox::Binary::accept(Visitor & visitor) const
{
	return visitor.visit(*this);
}

lox::Unary::Unary(Expr exp, Token op)
	: operand(std::move(exp)), operation(op)
{
}

/*
std::any lox::Unary::accept(Visitor<std::any> visitor) const
{
	return visitor.visit(*this);
}*/

std::any lox::Unary::accept(Visitor & visitor) const
{
	return visitor.visit(*this);
}

lox::Grouping::Grouping(Expr exp)
	: expression(std::move(exp))
{
}

/*std::any lox::Grouping::accept(Visitor<std::any> visitor) const
{
	return visitor.visit(*this);
}*/

std::any lox::Grouping::accept(Visitor & visitor) const
{
	return visitor.visit(*this);
}

lox::Operator::Operator(Token op)
	: operation(op)
{
}

/*std::any lox::Operator::accept(Visitor<std::any> visitor) const
{
	return visitor.visit(*this);
}*/

std::any lox::Operator::accept(Visitor & visitor) const
{
	return visitor.visit(*this);
}

lox::Value::Value(Literal val)
	: value(std::move(val))
{
}

/*std::any lox::Value::accept(Visitor<std::any> visitor) const
{
	return visitor.visit(*this);
}*/

std::any lox::Value::accept(Visitor & visitor) const
{
	return visitor.visit(*this);
}

lox::Conditional::Conditional(Expr cond, Expr lhs, Expr rhs)
	: condition(std::move(cond)), left(std::move(lhs)), right(std::move(rhs))
{
}

std::any lox::Conditional::accept(Visitor & visitor) const
{
	return visitor.visit(*this);
}
