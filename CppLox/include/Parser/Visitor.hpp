#pragma once

#include <any>

#include "Common.h"

namespace lox
{
	struct Expression;

	class Binary;
	class Unary;
	class Grouping;
	class Operator;
	class Value;

	struct ExprVisitor
	{
		// Static visitor functions
		virtual std::any visit(const Binary&) = 0;
		virtual std::any visit(const Unary&) = 0;
		virtual std::any visit(const Grouping&) = 0;
		virtual std::any visit(const Operator&) = 0;
		virtual std::any visit(const Value&) = 0;
	};


	class ASTPrinter : public ExprVisitor
	{
	public:
		std::string print(const Expression&);
		std::any visit(const Binary&) override;
		std::any visit(const Unary&) override;
		std::any visit(const Grouping&) override;
		std::any visit(const Operator&) override;
		std::any visit(const Value&) override;

	private:
		template <typename...Args>
		std::string parenthesize(std::string name, const Expression& first, const Args&...rest);

		std::string add_space(const Expression&);
	};
}

// template <typename R = void>
// struct ExprVisitor
// {
// 	// Static visitor functions
//	/*static*/ R visit(const Binary&);
//	/*static*/ R visit(const Unary&);
//	/*static*/ R visit(const Grouping&);
//	/*static*/ R visit(const Operator&);
//	/*static*/ R visit(const Value&);
// };
/*
struct ASTPrinter : public ExprVisitor<std::string>
{
	using Base = ExprVisitor<std::string>;
	using Base::visit;

	std::string print(const Expression&);
	std::string visit(const Binary & exp);
	std::string visit(const Unary & exp);
	std::string visit(const Grouping & exp);
	std::string visit(const Operator & exp);
	std::string visit(const Value & exp);
};*/