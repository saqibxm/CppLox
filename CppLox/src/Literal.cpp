#include "Literal.hpp"

using namespace lox;

const Literal Literal::None;

/*
Literal::Literal(Operand op) : literal(op)
{
	;
}

Literal::Literal(std::string_view str) : literal(static_cast<std::string>(str))
{
}

lox::Literal::Literal(std::nullptr_t)
	: literal(null)
{
}
*/
/*
lox::Literal::Literal(bool f) : literal(f)
{
}*/

/*auto Literal::str() const
{
}*/

/*
lox::Literal::Literal(Variant value) : literal(std::move(value))
{
}*/

void Literal::set(Operand op)
{
	this->emplace<Operand>(op);
}

void Literal::set(std::string_view id)
{
	this->emplace<std::string>(id);
}

void lox::Literal::set(std::nullptr_t)
{
	this->emplace<std::nullptr_t>(nullptr);
}

void lox::Literal::set(bool b)
{
	this->emplace<bool>(b);
}

void Literal::reset()
{
	// literal = Variant{};
	*this = nullptr;
}

/*
std::string Literal::get_identifier() const
{
	auto p = std::get_if<std::string>(&literal);

	if (!p) throw std::invalid_argument("variant does not hold specified type!");
	return *p;
}
*/
std::string Literal::get_strliteral() const
{
	return std::get<std::string>(*this);
}

lox::Operand Literal::get_number() const
{
	auto p = std::get_if<Operand>(this);
	if(!p)
		throw std::invalid_argument("variant does not contain operand");
	return *p;
}

bool lox::Literal::get_boolean() const
{
	auto p = std::get_if<bool>(this);

	if (!p) throw std::invalid_argument("variant does not hold specified type!");
	return *p;
}
