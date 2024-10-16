#pragma once

#include "Common/Expression.hpp"

/*
program -> statement* ;
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
		virtual std::any accept(Visitor&) = 0;
	};

	class Expression : public Statement
	{
	public:
		Expression(Expr&&);
		Expr expression;
		std::any accept(Visitor&) override;
	};

	class Print : public Statement
	{
	public:
		Print(Expr&&);
		Expr expression;

		std::any accept(Visitor&) override;
	};
}