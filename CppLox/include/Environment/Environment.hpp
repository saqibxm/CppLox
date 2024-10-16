#pragma once

#include <unordered_map>

#include "Common.h"
#include "Literal.hpp" // Object
#include "Token.hpp"

#include "Diagnostics/Diagnostics.hpp" // RuntimeError

namespace lox
{
	class Environment
	{
	public:
		Environment() =  default;

		void define(const std::string &name, const Object &init);
		Object get(const Token &name);
		// Object get(std::string_view name);

	private:
		std::unordered_map<std::string, Object> values;
	};
}