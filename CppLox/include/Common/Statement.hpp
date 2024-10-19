#pragma once

#include "Common/Expression.hpp"

// namespace lox::stmt { struct Statement; } // forward declared by visitor.hpp already

namespace lox {
	using Stmt = std::unique_ptr<stmt::Statement>;
	using StatementList = std::vector<Stmt>;
}

namespace lox::stmt
{
	using namespace lox;

	struct Statement
	{
		virtual ~Statement() = default;
		virtual void accept(StmtVisitor&) const = 0;
	};

	class Expression final : public Statement
	{
	public:
		Expression(Expr&&);
		Expr expression;
		void accept(StmtVisitor&) const override;
	};

	class Print final : public Statement
	{
	public:
		Print(Expr&&);
		Expr expression;

		void accept(StmtVisitor&) const override;
	};

	class Var : public Statement
	{
	public:
		Var(const Token&, Expr&&);

		void accept(StmtVisitor&) const override;

		Token name;
		Expr initializer;
	};

	class Block final : public Statement
	{
	public:
		Block(StatementList&&);
		void accept(StmtVisitor&) const override;

		StatementList statements;
	};

	class IfControl final : public Statement
	{
	public:
		IfControl(Expr&&, Stmt&&, Stmt&&);
		void accept(StmtVisitor&) const override;

		Expr condition;
		Stmt then_stmt;
		Stmt else_stmt;
	};

	class While final : public Statement
	{
	public:
		While(Expr&&, Stmt&&);
		void accept(StmtVisitor&) const override;

		Expr condition;
		Stmt body;
	};
}