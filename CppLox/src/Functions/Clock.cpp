#include "Literal.hpp"
#include "Functions/Clock.hpp"

lox::Clock lox::internal_clock;

lox::Object lox::Clock::call(Interpreter & interpreter, std::vector<Object>& arguments)
{
	auto duration = ClockType::now() - start;

	auto time = crn::duration_cast<Duration>(duration).count();
	// auto time = duration.count();
	return Object(static_cast<Operand>(time));
}

