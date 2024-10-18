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
	class Variable;
	class Assign;
}

namespace lox::stmt
{
	struct Statement;
	class Expression;
	class Print;
	class Var;
	class Block;
}

namespace lox
{
	struct ExprVisitor
	{
		// Static visitor functions
		virtual std::any visit(const expr::Binary&) = 0;
		virtual std::any visit(const expr::Unary&) = 0;
		virtual std::any visit(const expr::Conditional&) = 0;
		virtual std::any visit(const expr::Grouping&) = 0;
		virtual std::any visit(const expr::Operator&) = 0;
		virtual std::any visit(const expr::Value&) = 0;
		virtual std::any visit(const expr::Variable&) = 0;
		virtual std::any visit(const expr::Assign&) = 0;
	};

	struct StmtVisitor
	{
		virtual void visit(const stmt::Expression&) = 0;
		virtual void visit(const stmt::Print&) = 0;
		virtual void visit(const stmt::Var&) = 0;
		virtual void visit(const stmt::Block&) = 0;
	};

	class ASTPrinter : public ExprVisitor
	{
	public:
		std::string print(const expr::Expression&);
		std::any visit(const expr::Binary&) override;
		std::any visit(const expr::Unary&) override;
		std::any visit(const expr::Conditional&) override;
		std::any visit(const expr::Grouping&) override;
		std::any visit(const expr::Operator&) override;
		std::any visit(const expr::Value&) override;
		std::any visit(const expr::Variable&) override;
		std::any visit(const expr::Assign&) override;

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