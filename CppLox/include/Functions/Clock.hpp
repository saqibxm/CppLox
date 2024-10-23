#pragma once

#include "Callable.hpp"

namespace lox {

	struct Clock : public NativeFunction
	{
		Clock() : NativeFunction("clock") { }
		using ClockType = crn::steady_clock;
		using TimePoint = ClockType::time_point;
		using Duration = crn::duration<long long, std::milli>;
		// using Duration = crn::duration<Operand, std::milli>;

		static const inline TimePoint start = ClockType::now(); // capture the start time of the interpreter

		Object call(Interpreter &interpreter, const std::vector<Object> &arguments) override;
		unsigned arity() const noexcept override { return 0; }
		// std::string to_string() const { return "<native-fn>"; }
	} /*extern lox_clock*/;

	// extern Clock internal_clock; // this here is a global variable with static initialization so when you
	// include it in other TUs by that time it is initialized

	// inline NativeFunction::Ptr internal_clock = std::make_shared<Clock>();
	// extern NativeFunction::Ptr internal_clock; // hit by static initialization order fiasco

	// to counter one can provide a function which has static object of type and returns reference to it.
}

