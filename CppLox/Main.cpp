#include <iostream>

#include "Common.h"
#include "Lox.hpp"
#include "Literal.hpp"

#include "Parser/Visitor.hpp"
#include "Parser/Expression.hpp"
#include "Diagnostics/Diagnostics.hpp"

#include "Parser/RPNPrinter.hpp"

using namespace lox;

int main(int argc, char* argv[])
{
	std::ios::sync_with_stdio(false); // disable marker tracking with C stdio library functions

	lox::ASTPrinter ast;
	lox::RPNPrinter rpn;

	auto expr = new lox::Unary(lox::Expr(new lox::Value(3.14159265)), lox::Token(lox::TokenType::MINUS, "-", {}, 1));
	auto expression = Expr(new Binary(
		Expr(new lox::Unary(
			Expr(new Value(123.0)),
			Token(TokenType::MINUS, "-", {}, 1))),
		Token(TokenType::ASTERISK, "*", {}, 1),
		Expr(new Grouping(
			Expr(new Value(45.67))))));

	auto equation = Expr(
		new Unary(
		Expr(new Binary(
			Expr(new Binary(Expr(new Value(1)), Token(TokenType::PLUS, "+", {}, 1), Expr(new Value(2)))),
			Token(TokenType::ASTERISK, "*", {}, 1),
			Expr(new Binary(Expr(new Value(4)), Token(TokenType::ASTERISK, "-", {}, 1), Expr(new Value(3))))
		)),
			Token(TokenType::MINUS, "-", {}, 1)
		)
	);

	std::cout << ast.print(*expression) << std::endl;
	std::cout << rpn.print(*equation) << std::endl;
	delete expr;

	if (argc > 2)
	{
		std::cerr << "Usage: " << fs::path(argv[0]).filename().string() << " <file>\n";
	}
	else if (argc == 2) lox::RunFile(fs::path(argv[1]));
	else lox::RunPrompt();

	if (diagnostics.HasError()) return 65;
	if (diagnostics.HadRuntimeError()) return 75;
}