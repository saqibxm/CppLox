#pragma once

#include "Callable.hpp"
#include "Environment/Environment.hpp"
#include "Common/Statement.hpp"

namespace lox
{
	class LoxFunction : public Callable
	{
	public:
		LoxFunction(stmt::Function& decl);

		Object call(Interpreter&, const std::vector<Object>&) override;
		unsigned arity() const noexcept override { return declaration.parameters.size(); }
		std::string to_string() const override { return "<fn:" + declaration.name.lexeme + '>'; }
		const std::string& name() const { return declaration.name.lexeme; }

	private:
		stmt::Function declaration;
	};
}