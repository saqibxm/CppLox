#pragma once

#include "Common/Expression.hpp"

/*
program -> declaration* EOF ;

declaration -> varDecl | statement ;

varDecl -> IDENTIFIER ( "=" initializer )? ";" ;

statement -> expressionStmt| printStmt | block ;

block -> "{" declaration* "}" ;

expressionStmt -> expression ";" ;

printStmt -> "print" expressionStmt ;
*/

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
		virtual void accept(Visitor&) const = 0;
	};

	class Expression final : public Statement
	{
	public:
		Expression(Expr&&);
		Expr expression;
		void accept(Visitor&) const override;
	};

	class Print final : public Statement
	{
	public:
		Print(Expr&&);
		Expr expression;

		void accept(Visitor&) const override;
	};

	class Var : public Statement
	{
	public:
		Var(const Token&, Expr&&);

		void accept(Visitor&) const override;

		Token name;
		Expr initializer;
	};

	class Block final : public Statement
	{
	public:
		Block(StatementList&&);
		void accept(Visitor&) const override;

		StatementList statements;
	};
}