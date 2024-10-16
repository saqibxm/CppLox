#include "Environment/Environment.hpp"

void lox::Environment::define(const std::string &name, const Object &init)
{
	// doesnt check if the name already exists, choice to make it a redefintion error
	// can selectively disable error if running in REPL mode, otherwise let it throw an error.
	// values.insert({ name, init }); // doesnt insert if already exists
	values[name] = init;
}

lox::Object lox::Environment::get(const Token & name)
{
	// choices are to indicate syntax error, throw runtime error (used) return null.
	auto iter = values.find(name.lexeme);
	if (iter == values.end()) throw RuntimeError(name, '\"' + name.lexeme + "\" identifier not found, undefined variable.");
	return iter->second;
}
