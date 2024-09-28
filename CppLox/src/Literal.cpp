#include "Literal.hpp"

using namespace lox;

Literal::Literal(Literal::Operand op) : literal(op)
{
	;
}

Literal::Literal(std::string_view str) : literal(static_cast<std::string>(str))
{
}

/*auto Literal::get() const
{
}*/


std::string Literal::get_identifier() const
{
	auto p = std::get_if<std::string>(&literal);

	if (!p) throw std::invalid_argument("variant does not hold specified type!");
	return *p;
}

std::string Literal::get_strliteral() const
{
	return std::get<std::string>(literal);
}

Literal::Operand Literal::get_number() const
{
	auto p = std::get_if<Operand>(&literal);
	if(!p)
		throw std::invalid_argument("variant does not contain operand");
	return *p;
}