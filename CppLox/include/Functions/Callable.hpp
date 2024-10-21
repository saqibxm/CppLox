#pragma once

#include <string>
#include <type_traits>

#include "Common.h"

namespace lox
{
	struct Object;
	class Interpreter;

	struct Callable
	{
		using Ptr = std::add_pointer_t<Callable>;

		virtual Object call(Interpreter &interpreter, [[maybe_unused]] std::vector<Object> &arguments) = 0;
		virtual unsigned arity() const noexcept = 0;
		virtual std::string to_string() const {
			return "<0x" + std::to_string(reinterpret_cast<std::uintptr_t>(std::addressof(*this))) + ">";
		}
	};
}