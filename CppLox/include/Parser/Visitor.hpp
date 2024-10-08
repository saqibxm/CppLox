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

	template <typename R = void>
	struct ExprVisitor
	{
		// Static visitor functions
		/*static*/ R visit(const Binary&);
		/*static*/ R visit(const Unary&);
		/*static*/ R visit(const Grouping&);
		/*static*/ R visit(const Operator&);
		/*static*/ R visit(const Value&);
	};

	struct ExperimentalVisitor
	{
		// Static visitor functions
		virtual std::any visit(const Binary&) { return std::string("Binary"); }
		virtual std::any visit(const Unary&) { return std::string("Binary"); }
		virtual std::any visit(const Grouping&) { return std::string("Binary"); }
		virtual std::any visit(const Operator&) { return std::string("Binary"); }
		virtual std::any visit(const Value&) { return std::string("Binary"); }
	};


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
	};

	class ExperimentalPrinter : public ExperimentalVisitor
	{
		using Base = ExprVisitor<std::string>;

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