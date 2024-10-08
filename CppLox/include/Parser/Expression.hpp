#pragma once

#include <any>
#include <memory>

#include "Common.h"
#include "Visitor.hpp"
#include "Token.hpp"

/*
expression -> literal
 | unary
 | binary
 | grouping ;
literal -> NUMBER | STRING | "true" | "false" | "nil" ;
grouping -> "(" expression ")" ;
unary -> ( "-" | "!" ) expression ;
binary -> expression operator expression ;
operator -> "==" | "!=" | "<" | "<=" | ">" | ">="
 | "+" | "-" | "*" | "/" ;
*/


namespace lox {
	struct Expression
	{
		// virtual std::any accept(ExprVisitor<std::any>) const = 0;
		virtual std::any accept(ExperimentalVisitor &visitor) const = 0;
		// virtual std::string apply(ExprVisitor<std::string>) const = 0; // for pretty printer
	};
	using ExprNode = std::unique_ptr<Expression>;

	class Operator final : public Expression
	{
	public:
		Operator(Token op);
		// std::any accept(ExprVisitor<std::any> visitor) const override;
		std::any accept(ExperimentalVisitor &visitor) const override;

		Token operation;
	};

	class Binary final : public Expression
	{
	public:
		Binary(ExprNode lhs, Token op, ExprNode rhs);
		// std::any accept(ExprVisitor<std::any> visitor) const override;
		std::any accept(ExperimentalVisitor &visitor) const override;

	// private:
		ExprNode left, right;
		Token operation;
	};

	class Unary final : public Expression
	{
	public:
		Unary(ExprNode exp, Token op);
		// std::any accept(ExprVisitor<std::any> visitor) const override;
		std::any accept(ExperimentalVisitor &visitor) const override;

	// private:
		ExprNode operand;
		Token operation;
	};

	class Grouping final : public Expression
	{
	public:
		Grouping(ExprNode exp);
		// std::any accept(ExprVisitor<std::any> visitor) const override;
		std::any accept(ExperimentalVisitor &visitor) const override;

	// private:
		ExprNode expression;
	};

	class Value final : public Expression
	{
	public:
		Value(Literal val);
		// std::any accept(ExprVisitor<std::any> visitor) const override;
		std::any accept(ExperimentalVisitor &visitor) const override;

		Literal value;
	};
}