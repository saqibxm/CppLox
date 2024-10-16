#pragma once

#include "Common/Expression.hpp"

/*
program -> declaration* EOF ;
declaration -> varDecl | statement ;
varDecl -> IDENTIFIER ( "=" initializer )? ";" ;
statement -> expressionStmt | printStmt ;
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
		virtual void accept(Visitor&) = 0;
	};

	class Expression : public Statement
	{
	public:
		Expression(Expr&&);
		Expr expression;
		void accept(Visitor&) override;
	};

	class Print : public Statement
	{
	public:
		Print(Expr&&);
		Expr expression;

		void accept(Visitor&) override;
	};

	class Var : public Statement
	{
	public:
		Var(const Token&, Expr&&);

		void accept(Visitor&) override;

		Token name;
		Expr initializer;
	};
}