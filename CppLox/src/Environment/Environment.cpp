#include "Environment/Environment.hpp"

void lox::Environment::define(const std::string &name, const Object &init)
{
	// doesn't check if the name already exists, choice to make it a redefintion error
	// can selectively disable error if running in REPL mode, otherwise let it throw an error.
	// values.insert({ name, init }); // doesnt insert if already exists

	if (values.find(name) != values.end() && init.empty()) return; // donot let varibale be uninitialized on purpose
	// persist the value it has unless an initializer is provided
	values[name] = init;
}

const lox::Object& lox::Environment::retrieve(const Token & name) const
{
	const auto& obj = get(name);
	if (obj.empty()) throw RuntimeError(name, "Uninitialized variable '" + name.lexeme + "' used.");
	return obj;
}

lox::Object lox::Environment::retrieve(const Storage::key_type & name)
{
	return get(Token(TokenType::IDENTIFIER, name, {}, -1));
	// return get(name);
}

void lox::Environment::assign(const Token & name, const Object & val)
{
	set(name, val);
}

void lox::Environment::assign(const Storage::key_type & name, const Object & val)
{
	set(Token(TokenType::IDENTIFIER, name, {}, -1), val);
}

void lox::Environment::reserve(Storage::size_type n)
{
	values.reserve(n);
}

const lox::Object& lox::Environment::get(const Token & name) const
{
	// choices are to indicate syntax error, throw runtime error (used) return null.
	auto iter = values.find(name.lexeme);
	if (iter == values.end())
	{
		if(!enclosing)
			throw RuntimeError(name, '\"' + name.lexeme + "\" identifier not found, undefined variable.");
		else return enclosing->get(name);
	}
	return iter->second;
}

void lox::Environment::set(const Token & name, const Object & val)
{
	auto it = values.find(name.lexeme);
	if (it == values.end())
	{
		if(!enclosing) throw lox::RuntimeError(name, "Identifier \'" + name.lexeme + "\' is not defined.");
		enclosing->set(name, val);
	}
	else it->second = val; // without the else this is a logic error
}

/*
std::optional<lox::Object> lox::Environment::get(const std::string &name) const
{
	auto iter = values.find(name);
	if (iter == values.end())
	{
		if (!enclosing)
			return std::nullopt;
		else return enclosing->get(name);
	}
	return iter->second;
}
*/

/*
lox::Object lox::Environment::get(const Token & name)
{
	// choices are to indicate syntax error, throw runtime error (used) return null.
	auto iter = values.find(name.lexeme);
	if (iter == values.end()) throw RuntimeError(name, '\"' + name.lexeme + "\" identifier not found, undefined variable.");
	return iter->second;
}

void lox::Environment::set(const Token & name, const Object & val)
{
	auto it = values.find(name.lexeme);
	if (it == values.end()) throw lox::RuntimeError(name, "Identifier \'" + name.lexeme + "\' is not defined.");
	it->second = val;
}

*/