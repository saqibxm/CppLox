#include "Environment/Environment.hpp"

void lox::Environment::define(const std::string &name, const Object &init)
{
	// doesnt check if the name already exists, choice to make it a redefintion error
	// can selectively disable error if running in REPL mode, otherwise let it throw an error.
	// values.insert({ name, init }); // doesnt insert if already exists
	values[name] = init;
}

lox::Object lox::Environment::retrieve(const Token & name)
{
	return get(name);
}

void lox::Environment::assign(const Token & name, const Object & val)
{
	set(name, val);
}

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
