#pragma once

#include "Common/Expression.hpp"

// namespace lox::stmt { struct Statement; } // forward declared by visitor.hpp already

namespace lox {
	using Stmt = std::unique_ptr<stmt::Statement>;
	using StatementList = std::vector<Stmt>;
	using TokenList = std::vector<Token>;

	struct BreakExcept final : public std::exception
	{
		BreakExcept() = default;
	};

	struct ContinueExcept : public std::exception
	{
		ContinueExcept() = default; // : std::exception("continue encountered") {}
	};
}

namespace lox::stmt
{
	using namespace lox;

	struct Statement
	{
		virtual ~Statement() = default;
		// virtual Statement* clone() const = 0;
		// virtual void accept(StmtVisitor&) const = 0;
		virtual void accept(StmtVisitor&) = 0;
	};

	class Expression final : public Statement
	{
	public:
		Expression(Expr&&);
		Expr expression;
		void accept(StmtVisitor&) override;
	};

	class Print final : public Statement
	{
	public:
		Print(Expr&&);
		Expr expression;

		void accept(StmtVisitor&) override;
	};

	class Var : public Statement
	{
	public:
		Var(const Token&, Expr&&);

		void accept(StmtVisitor&) override;

		Token name;
		Expr initializer;
	};

	class Block final : public Statement
	{
	public:
		Block(StatementList&&);
		void accept(StmtVisitor&) override;

		StatementList statements;
	};

	class IfControl final : public Statement
	{
	public:
		IfControl(Expr&&, Stmt&&, Stmt&&);
		void accept(StmtVisitor&) override;

		Expr condition;
		Stmt then_stmt;
		Stmt else_stmt;
	};

	class While final : public Statement
	{
	public:
		While(Expr&&, Stmt&&);
		void accept(StmtVisitor&) override;

		Expr condition;
		Stmt body;
	};

	class LoopControl final : public Statement
	{
	public:
		enum Type { CONTINUE, BREAK };

		LoopControl(Type);
		void accept(StmtVisitor&) override;

		Type control;
	};

	class Function final : public Statement
	{
	public:
		Function(const Token&, const TokenList&, StatementList&&);
		void accept(StmtVisitor&) override;

		Token name;
		TokenList parameters;
		StatementList body;
		// Block body;
	};
}