#include "Literal.hpp"
#include "Functions/Callable.hpp"

using namespace lox;

const Object Object::None;

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

std::string lox::Object::str() const {
	auto s = std::visit([](const auto &v) -> std::string {
		using T = std::decay_t<decltype(v)>;
		if constexpr (std::is_same_v<T, Operand>)
		{
			auto str = std::to_string(v);
			if (std::string::size_type pos = str.find_last_not_of('0');
				pos != std::string::npos/* && str[pos] == '.'*/)
			{
				if (str[pos] != '.') ++pos;
				str.erase(pos, str.length() - pos);
			}
			return str;
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			return v;
		}
		else if constexpr (std::is_same_v<T, Callable::Ptr>)
		{
			return v->to_string();
			// return std::to_string((unsigned)v);
		}
		else if constexpr (std::is_same_v<T, std::nullptr_t>)
		{
			return "null";
		}
		else if constexpr (std::is_same_v<T, bool>)
		{
			return v ? "true" : "false";
		}
		else return {};
	}, static_cast<const Base&>(*this));

	return s;
}

void Object::set(Operand op)
{
	this->emplace<Operand>(op);
}

void Object::set(std::string_view id)
{
	this->emplace<std::string>(id);
}

void lox::Object::set(std::nullptr_t)
{
	this->emplace<std::nullptr_t>(nullptr);
}

void lox::Object::set(bool b)
{
	this->emplace<bool>(b);
}

void lox::Object::set(Callable & callable)
{
	this->emplace<Callable::Ptr>(std::addressof(callable));
}

void Object::reset()
{
	// literal = Variant{};
	// *this = nullptr;
	this->emplace<0>();
}

/*
std::string Literal::get_identifier() const
{
	auto p = std::get_if<std::string>(&literal);

	if (!p) throw std::invalid_argument("variant does not hold specified type!");
	return *p;
}
*/
std::string Object::get_strliteral() const
{
	return std::get<std::string>(*this);
}

lox::Operand Object::get_number() const
{
	auto p = std::get_if<Operand>(this);
	if(!p)
		throw std::invalid_argument("variant does not contain operand");
	return *p;
}

bool lox::Object::get_boolean() const
{
	auto p = std::get_if<bool>(this);

	if (!p) throw std::invalid_argument("variant does not hold specified type!");
	return *p;
}
