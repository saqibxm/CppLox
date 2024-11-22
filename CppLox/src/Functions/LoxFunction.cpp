#include "Functions/LoxFunction.hpp"

lox::LoxFunction::LoxFunction(stmt::Function & decl, Environment::Ptr closure)
	: declaration(std::move(decl)), closure(closure)
{
}

lox::Object lox::LoxFunction::call(Interpreter &interpreter, const std::vector<Object> &arguments)
{
	Environment::Ptr environment = std::make_shared<Environment>(closure);
	// A new scope whose parent scope is global which means that its only scope is the function itself
	// and scopes in which the function is defined
	for (std::size_t i = 0; i < declaration.parameters.size(); ++i) // init in range based for is C++20
		environment->define(declaration.parameters[i].lexeme, arguments[i]);
	// initialized all identifiers found in function declaration with the value of arguments passed

	try {
		interpreter.execute_block(declaration.body, environment);
	} catch(const lox::ReturnExcept &ret) {
		return ret.value;
	}
	return nullptr; // returns null
}
