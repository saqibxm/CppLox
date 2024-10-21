#pragma once

#include "Callable.hpp"

namespace lox {

	struct Clock : public Callable
	{
		using ClockType = crn::steady_clock;
		using TimePoint = ClockType::time_point;
		using Duration = crn::duration<long long, std::milli>;
		// using Duration = crn::duration<Operand, std::milli>;

		static const inline TimePoint start = ClockType::now(); // capture the start time of the interpreter

		Object call(Interpreter &interpreter, std::vector<Object> &arguments) override;
		unsigned arity() const noexcept override { return 0; }
		std::string to_string() const { return "<native-fn>"; }
	} /*extern lox_clock*/;

	extern Clock internal_clock;
}

