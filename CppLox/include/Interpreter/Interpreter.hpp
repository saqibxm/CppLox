#pragma once

#include "Parser/Expression.hpp"
#include "Parser/Visitor.hpp"
#include "Diagnostics/Diagnostics.hpp"

namespace lox
{
	struct RuntimeError : public std::runtime_error
	{
		RuntimeError(const Token &tok, const std::string &msg)
			: runtime_error(msg), token(tok)
		{
			;
		}

		Token token;
	};

	class Interpreter : public Visitor
	{
	public:
		void Interpret(const Expression&);

		Literal evaluate(const Expression&);
		std::any visit(const Binary&) override;
		std::any visit(const Unary&) override;
		std::any visit(const Conditional&) override;
		std::any visit(const Grouping&) override;
		std::any visit(const Operator&) override;
		std::any visit(const Value&) override;

	private:
		static Operand factorial(Operand);
		static bool is_true(const Literal&);
		static bool are_equal(const Literal&, const Literal&);
		static bool equal_impl(const Literal&, const Literal&);

		static void validate_number(const Token&, const Literal&) noexcept(false);
		static void check_number_operands(const Token&, const Literal&, const Literal&) noexcept(false);
	};

}