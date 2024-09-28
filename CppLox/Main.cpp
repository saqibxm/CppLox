#include <iostream>

#include "Common.h"
#include "Lox.hpp"
#include "Literal.hpp"

int main(int argc, char* argv[])
{
	std::ios::sync_with_stdio(false); // disable marker tracking with C stdio library functions

	if (argc > 2)
	{
		std::cerr << "Usage: " << fs::path(argv[0]).filename().string() << " <file>\n";
	}
	else if (argc == 2) lox::RunFile(fs::path(argv[1]));
	else lox::RunPrompt();
}