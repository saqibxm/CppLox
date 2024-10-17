#pragma once

#include "Common/Expression.hpp"
#include "Common/Statement.hpp"
#include "Common/Visitor.hpp"
#include "Diagnostics/Diagnostics.hpp"
#include "Environment/Environment.hpp"

namespace lox
{
	class Interpreter : public expr::Visitor, public stmt::Visitor
	{
	public:
		void Interpret(const StatementList&);

		std::any visit(const expr::Binary&) override;
		std::any visit(const expr::Unary&) override;
		std::any visit(const expr::Conditional&) override;
		std::any visit(const expr::Grouping&) override;
		std::any visit(const expr::Operator&) override;
		std::any visit(const expr::Value&) override;
		std::any visit(const expr::Variable&) override;
		std::any visit(const expr::Assign&) override;

		void visit(stmt::Expression&) override;
		void visit(stmt::Print&) override;
		void visit(stmt::Var&) override;

	private:
		Environment environment;
		void execute(stmt::Statement&);
		Literal evaluate(const expr::Expression&);

		static bool is_true(const Literal&);
		static bool are_equal(const Literal&, const Literal&);
		static bool equal_impl(const Literal&, const Literal&);

		static void validate_number(const Token&, const Literal&) noexcept(false);
		static void check_number_operands(const Token&, const Literal&, const Literal&) noexcept(false);
	};

}