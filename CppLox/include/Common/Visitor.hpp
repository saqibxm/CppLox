#pragma once

#include <any>

#include "Common.h"

namespace lox::expr
{
	struct Expression;

	class Binary;
	class Unary;
	class Conditional;
	class Grouping;
	class Operator;
	class Value;

	struct Visitor
	{
		// Static visitor functions
		virtual std::any visit(const Binary&) = 0;
		virtual std::any visit(const Unary&) = 0;
		virtual std::any visit(const Conditional&) = 0;
		virtual std::any visit(const Grouping&) = 0;
		virtual std::any visit(const Operator&) = 0;
		virtual std::any visit(const Value&) = 0;
	};
}

namespace lox::stmt
{
	struct Statement;
	class Expression;
	class Print;

	struct Visitor
	{
		virtual std::any visit(Expression&) = 0;
		virtual std::any visit(Print&) = 0;
	};
}

namespace lox
{
	class ASTPrinter : public expr::Visitor
	{
	public:
		std::string print(const expr::Expression&);
		std::any visit(const expr::Binary&) override;
		std::any visit(const expr::Unary&) override;
		std::any visit(const expr::Conditional&) override;
		std::any visit(const expr::Grouping&) override;
		std::any visit(const expr::Operator&) override;
		std::any visit(const expr::Value&) override;

	private:
		template <typename...Args>
		std::string parenthesize(std::string name, const expr::Expression& first, const Args&...rest);

		std::string add_space(const expr::Expression&);
	};
}

// template <typename R = void>
// struct Visitor
// {
// 	// Static visitor functions
//	/*static*/ R visit(const Binary&);
//	/*static*/ R visit(const Unary&);
//	/*static*/ R visit(const Grouping&);
//	/*static*/ R visit(const Operator&);
//	/*static*/ R visit(const Value&);
// };
/*
struct ASTPrinter : public Visitor<std::string>
{
	using Base = Visitor<std::string>;
	using Base::visit;

	std::string print(const Expression&);
	std::string visit(const Binary & exp);
	std::string visit(const Unary & exp);
	std::string visit(const Grouping & exp);
	std::string visit(const Operator & exp);
	std::string visit(const Value & exp);
};*/

/*
template <typename R>
	struct XVisitor
	{
		virtual R visit(const Binary&) const = 0;
		virtual R visit(const Unary&) const = 0;
		virtual R visit(const Conditional&) const = 0;
		virtual R visit(const Grouping&) const = 0;
		virtual R visit(const Operator&) const = 0;
		virtual R visit(const Value&) const = 0;
	};

	struct XPrinter : public XVisitor<std::string>
	{
	public:
		std::string print(const Expression&) const;
		std::string visit(const Binary & exp) const override;
		std::string visit(const Unary & exp) const override;
		std::string visit(const Conditional & exp) const override;
		std::string visit(const Grouping & exp) const override;
		std::string visit(const Operator & exp) const override;
		std::string visit(const Value & exp) const override;

	private:
		std::string add_space(const Expression&) const;

		template <typename...Args>
		std::string parenthesize(std::string name, const Expression& first, const Args&...rest) const;
	};
*/