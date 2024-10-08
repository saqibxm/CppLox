#include "Parser/Expression.hpp"

using namespace lox;

lox::Binary::Binary(ExprNode lhs, Token op, ExprNode rhs)
	: left(std::move(lhs)), right(std::move(rhs)), operation(std::move(op))
{
}

/*
std::any lox::Binary::accept(ExprVisitor<std::any> visitor) const
{
	return visitor.visit(*this);
}*/

std::any lox::Binary::accept(ExperimentalVisitor & visitor) const
{
	return visitor.visit(*this);
}

lox::Unary::Unary(ExprNode exp, Token op)
	: operand(std::move(exp)), operation(op)
{
}

/*
std::any lox::Unary::accept(ExprVisitor<std::any> visitor) const
{
	return visitor.visit(*this);
}*/

std::any lox::Unary::accept(ExperimentalVisitor & visitor) const
{
	return visitor.visit(*this);
}

lox::Grouping::Grouping(ExprNode exp)
	: expression(std::move(exp))
{
}

/*std::any lox::Grouping::accept(ExprVisitor<std::any> visitor) const
{
	return visitor.visit(*this);
}*/

std::any lox::Grouping::accept(ExperimentalVisitor & visitor) const
{
	return visitor.visit(*this);
}

lox::Operator::Operator(Token op)
	: operation(op)
{
}

/*std::any lox::Operator::accept(ExprVisitor<std::any> visitor) const
{
	return visitor.visit(*this);
}*/

std::any lox::Operator::accept(ExperimentalVisitor & visitor) const
{
	return visitor.visit(*this);
}

lox::Value::Value(Literal val)
	: value(std::move(val))
{
}

/*std::any lox::Value::accept(ExprVisitor<std::any> visitor) const
{
	return visitor.visit(*this);
}*/

std::any lox::Value::accept(ExperimentalVisitor & visitor) const
{
	return visitor.visit(*this);
}
