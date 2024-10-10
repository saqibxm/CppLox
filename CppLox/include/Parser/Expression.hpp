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
		virtual ~Expression() = default;
		virtual std::any accept(ExprVisitor&) const = 0;
	};
	using ExprNode = std::unique_ptr<Expression>;

	class Operator final : public Expression
	{
	public:
		Operator(Token op);
		std::any accept(ExprVisitor &visitor) const override;
		// std::any accept(ExperimentalVisitor &visitor) const override;

		Token operation;
	};

	class Binary final : public Expression
	{
	public:
		Binary(ExprNode lhs, Token op, ExprNode rhs);
		std::any accept(ExprVisitor &visitor) const override;
		// std::any accept(ExperimentalVisitor &visitor) const override;

	// private:
		ExprNode left, right;
		Token operation;
	};

	class Unary final : public Expression
	{
	public:
		Unary(ExprNode exp, Token op);
		std::any accept(ExprVisitor &visitor) const override;
		// std::any accept(ExperimentalVisitor &visitor) const override;

	// private:
		ExprNode operand;
		Token operation;
	};

	class Grouping final : public Expression
	{
	public:
		Grouping(ExprNode exp);
		std::any accept(ExprVisitor &visitor) const override;

	// private:
		ExprNode expression;
	};

	class Value final : public Expression
	{
	public:
		explicit Value(Literal val);
		template <typename T, std::enable_if_t<std::is_same_v<T, bool>, int> = 0>
		explicit Value(T b) : Value(Literal{ std::in_place_type<bool>, b }) {}
		template <typename T,
			typename = std::enable_if_t<std::is_arithmetic_v<T> && !std::is_same_v<T, bool>>>
		Value(T val) : Value(Literal{ std::in_place_type<Operand>, val }) {}
		Value(const std::string &str) : Value(Literal{str}) {}
		Value(std::nullptr_t) : Value(Literal{nullptr}) {}

		std::any accept(ExprVisitor &visitor) const override;

		Literal value;
	};
}

/*
template <typename T,
			typename X = std::enable_if_t<std::conjunction(std::negation(std::is_same_v<T, bool>), std::is_convertible_v<T, Operand>)>>
		Value(T val) : value(std::in_place_type<double>, val) {}

Value(T val) : Value(Literal::Variant{ std::in_place_type<Operand>, val }) {}


class Value final : public Expression
	{
	public:
		explicit Value(Literal val);
		template <typename T, std::enable_if_t<std::is_same_v<T, bool>, int> = 0>
		explicit Value(T b) : Value(Literal{ std::in_place_type<bool>, b }) {}
		template <typename T,
			typename = std::enable_if_t<std::is_arithmetic_v<T> && !std::is_same_v<T, bool>>>
		Value(T val) : Value(Literal{ std::in_place_type<Operand>, val }) {}
		Value(const std::string &str) : Value(Literal{str}) {}
		Value(std::nullptr_t) : Value(Literal{Literal::null}) {}

		std::any accept(ExprVisitor &visitor) const override;

		Literal value;
	};
*/