#include "Common/Expression.hpp"

using namespace lox;

expr::Binary::Binary(Expr lhs, Token op, Expr rhs)
	: left(std::move(lhs)), right(std::move(rhs)), operation(std::move(op))
{
}

/*
std::any expr::Binary::accept(Visitor<std::any> visitor) const
{
	return visitor.visit(*this);
}*/

std::any expr::Binary::accept(Visitor & visitor) const
{
	return visitor.visit(*this);
}

expr::Unary::Unary(Expr exp, Token op)
	: operand(std::move(exp)), operation(op)
{
}

/*
std::any expr::Unary::accept(Visitor<std::any> visitor) const
{
	return visitor.visit(*this);
}*/

std::any expr::Unary::accept(Visitor & visitor) const
{
	return visitor.visit(*this);
}

expr::Grouping::Grouping(Expr exp)
	: expression(std::move(exp))
{
}

/*std::any expr::Grouping::accept(Visitor<std::any> visitor) const
{
	return visitor.visit(*this);
}*/

std::any expr::Grouping::accept(Visitor & visitor) const
{
	return visitor.visit(*this);
}

expr::Operator::Operator(Token op)
	: operation(op)
{
}

/*std::any expr::Operator::accept(Visitor<std::any> visitor) const
{
	return visitor.visit(*this);
}*/

std::any expr::Operator::accept(Visitor & visitor) const
{
	return visitor.visit(*this);
}

expr::Value::Value(Literal val)
	: value(std::move(val))
{
}

/*std::any expr::Value::accept(Visitor<std::any> visitor) const
{
	return visitor.visit(*this);
}*/

std::any expr::Value::accept(Visitor & visitor) const
{
	return visitor.visit(*this);
}


expr::Conditional::Conditional(Expr cond, Expr lhs, Expr rhs)
	: condition(std::move(cond)), left(std::move(lhs)), right(std::move(rhs))
{
}

std::any expr::Conditional::accept(Visitor & visitor) const
{
	return visitor.visit(*this);
}
