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
		virtual std::any accept(Visitor&) const = 0;

		/*
		template <typename R>
		R accept(XVisitor<R> &visitor);
		*/

	protected:
		// virtual std::any accept(XVisitor<std::any> &visitor) const = 0;
	};
	using Expr = std::unique_ptr<Expression>;

	class Operator final : public Expression
	{
	public:
		Operator(Token op);
		std::any accept(Visitor &visitor) const override;

		Token operation;
	};

	class Binary final : public Expression
	{
	public:
		Binary(Expr lhs, Token op, Expr rhs);
		std::any accept(Visitor &visitor) const override;
		// std::any accept(ExperimentalVisitor &visitor) const override;

	// private:
		Expr left, right;
		Token operation;
	};

	class Unary final : public Expression
	{
	public:
		Unary(Expr exp, Token op);
		std::any accept(Visitor &visitor) const override;
		// std::any accept(ExperimentalVisitor &visitor) const override;

	// private:
		Expr operand;
		Token operation;
	};

	class Conditional final : public Expression
	{
	public:
		Conditional(Expr cond, Expr lhs, Expr rhs);
		std::any accept(Visitor &visitor) const override;

	// private:
		Expr condition;
		Expr left, right;
	};

	class Grouping final : public Expression
	{
	public:
		Grouping(Expr exp);
		std::any accept(Visitor &visitor) const override;

	// private:
		Expr expression;
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

		std::any accept(Visitor &visitor) const override;

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

		std::any accept(Visitor &visitor) const override;

		Literal value;
	};
*/