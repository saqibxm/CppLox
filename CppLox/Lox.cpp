#include <string>
#include <iostream>
#include <iterator>
#include <fstream>

#include "Lox.hpp" // includes common.h
#include "Lexer/Lexer.hpp"

#include "Parser/Parser.hpp"

void lox::Run(const std::string &source)
{
	// the interpreter's main work
	static Lexer lexer;

	lexer.Open(source);
	lexer.Reset();

	diagnostics = source;

	auto tokens = lexer.Scan();
	Parser parser(tokens);

	auto Expr = parser.Parse();

	if (diagnostics.HasError()) return;

	std::cout << "AST: " << ASTPrinter().print(*Expr) << std::endl;
}

void lox::RunPrompt()
{
	std::string line;

	while (std::cout << ">>> " && std::getline(std::cin, line))
	{
		Run(line);
	}
}


void lox::RunFile(const fs::path &filename)
{
	std::ifstream file(filename);

	if (!file.is_open()) // !file
	{
		std::cerr << "Unable to open source file {" << filename.string() << "}\n";
		return;
	}

	std::string source(std::istreambuf_iterator<std::ifstream::char_type>(file), {});
	Run(source);
}