#pragma once

#include "Common/Expression.hpp"
#include "Common/Statement.hpp"
#include "Common/Visitor.hpp"
#include "Diagnostics/Diagnostics.hpp"
#include "Environment/Environment.hpp"

namespace lox
{
	class Interpreter : public ExprVisitor, public StmtVisitor
	{
		friend class LoxFunction;
		const Environment::Ptr globals = std::make_shared<Environment>(); // explicitly shared_ptr, dependant domain.

	public:
		Interpreter();
		void Interpret(const StatementList&);

		std::any visit(const expr::Binary&) override;
		std::any visit(const expr::Unary&) override;
		std::any visit(const expr::Conditional&) override;
		std::any visit(const expr::Grouping&) override;
		std::any visit(const expr::Operator&) override;
		std::any visit(const expr::Value&) override;
		std::any visit(const expr::Variable&) override;
		std::any visit(const expr::Assign&) override;
		std::any visit(const expr::Logical&) override;
		std::any visit(const expr::Call&) override;

		void visit(const stmt::Expression&) override;
		void visit(const stmt::Print&) override;
		void visit(const stmt::Var&) override;
		void visit(const stmt::Block&) override;
		void visit(const stmt::IfControl&) override;
		void visit(const stmt::While&) override;
		void visit(const stmt::LoopControl&) override;
		void visit(const stmt::Function&) override;
		void visit(const stmt::Return&) override;

	private:
		Environment::Ptr environment;

		void execute(stmt::Statement&);
		void execute_block(const StatementList& list, Environment::Ptr env);
		Object evaluate(const expr::Expression&);

		static bool is_true(const Object&);
		static bool are_equal(const Object&, const Object&);
		static bool equal_impl(const Object&, const Object&);

		static void validate_number(const Token&, const Object&) noexcept(false);
		static void check_number_operands(const Token&, const Object&, const Object&) noexcept(false);
	};

}

/*
inline lox::Environment::Ptr& global_env()
	{
		static lox::Environment::Ptr globals = std::make_shared<lox::Environment>();
		return globals; // circumvent static init order fiasco
	}
*/