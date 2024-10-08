#include <iostream>

#include "Common.h"
#include "Lox.hpp"
#include "Literal.hpp"

#include "Parser/Visitor.hpp"
#include "Parser/Expression.hpp"

using namespace lox;

int main(int argc, char* argv[])
{
	std::ios::sync_with_stdio(false); // disable marker tracking with C stdio library functions

	lox::ExperimentalPrinter printer;

	auto expr = new lox::Unary(lox::ExprNode(new lox::Value(3.14159265)), lox::Token(lox::TokenType::MINUS, "-", {}, 1));
	auto expression = ExprNode(new Binary(
		ExprNode(new lox::Unary(
			ExprNode(new Value(123)),
			Token(TokenType::MINUS, "-", {}, 1))),
		Token(TokenType::ASTERISK, "*", {}, 1),
		ExprNode(new Grouping(
			ExprNode(new Value(45.67))))));

	std::cout << printer.print(*expression) << std::endl;
	delete expr;

	if (argc > 2)
	{
		std::cerr << "Usage: " << fs::path(argv[0]).filename().string() << " <file>\n";
	}
	else if (argc == 2) lox::RunFile(fs::path(argv[1]));
	else lox::RunPrompt();
}