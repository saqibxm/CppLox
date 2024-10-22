#pragma once

#include "Callable.hpp"

namespace lox
{
	struct Exit : public NativeFunction // creat a base class for native functions
	{
		Exit() : NativeFunction("exit") { }
		Object call(Interpreter &interpreter, [[maybe_unused]] const std::vector<Object> &arguments) override {
			auto &arg = arguments.front();
			if (!std::holds_alternative<Operand>(arg))
				throw RuntimeError(Token(TokenType::IDENTIFIER, arg.str(), arg, 0), "function requires integer argument.");
			// workaround for now
			auto code = static_cast<int>(arg.get_number());
			std::exit(code);

			UNREACHABLE();
			return {};
		}
		unsigned arity() const noexcept override { return 1; }

	} /*inline lox_exit*/;
	// extern NativeFunction::Ptr internal_exit = std::make_shared<Exit>();
}