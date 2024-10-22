#include "Functions/Clock.hpp"

// lox::Clock lox::internal_clock;
// extern lox::NativeFunction::Ptr lox::internal_clock = std::make_shared<lox::Clock>();

lox::Object lox::Clock::call(Interpreter & interpreter, const std::vector<Object>& arguments)
{
	auto duration = ClockType::now() - start;

	auto time = crn::duration_cast<Duration>(duration).count();
	// auto time = duration.count();
	return Object(static_cast<Operand>(time));
}

