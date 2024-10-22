#pragma once

#include <string>
#include <type_traits>

#include "Common.h"
#include "Literal.hpp"

namespace lox
{
	class Interpreter;

	struct Callable
	{
		using RawPtr = std::add_pointer_t<Callable>;
		using Ptr = CallablePtr; // std::shared_ptr<Callable>;

		virtual Object call(Interpreter &interpreter, [[maybe_unused]] const std::vector<Object> &arguments) = 0;
		virtual unsigned arity() const noexcept = 0;
		virtual std::string to_string() const {
			// to be converted to hex, TODO use fmtlib
			return "<0x" + std::to_string(reinterpret_cast<std::uintptr_t>(std::addressof(*this))) + ">";
		}
	};

	struct NativeFunction : public Callable
	{
		using Ptr = std::shared_ptr<NativeFunction>;

		NativeFunction(const std::string&);
		const std::string name;

		virtual std::string to_string() const override {
			return "<" + name + ":native-fn>";
		}
	};
}