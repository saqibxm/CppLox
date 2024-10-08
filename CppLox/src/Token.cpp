#include "Token.hpp"

std::ostream& lox::operator<<(std::ostream &os, const lox::Token &token)
{
	std::string literal = token.literal.get();
	// if (auto o = token.literal) literal = o->get();

	return os << token.lexeme << ' ' << literal;
}