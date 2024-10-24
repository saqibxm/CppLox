#pragma once

#include <random>

#include "Callable.hpp"

namespace lox
{
	struct Random : public NativeFunction
	{
		using RandEngine = std::default_random_engine;
		using Distribution = std::uniform_real_distribution<double>;

		static inline RandEngine engine{};
		static inline Distribution distribution{};

		Random() : NativeFunction("random") {}
		Object call(Interpreter&, const std::vector<Object>&) override {

			return distribution(engine);
		}
		unsigned arity() const noexcept override { return 0; } // returns 0 to 1 exclusive
	};
}