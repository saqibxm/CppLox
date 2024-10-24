#pragma once

#include <filesystem>
#include <chrono>

#if defined(_WIN32) && defined(_MSC_VER)
#define UNREACHABLE() __assume(false);
#else
#define UNREACHABLE() __builtin_unreachable()
#endif

namespace fs = std::filesystem;
namespace crn = std::chrono;

namespace lox {
	inline bool repl = false;
	using Operand = double;

#pragma region DEBUG_INIDCATOR
	constexpr const bool debug =
#ifdef NDEBUG
		false
#else
		true
#endif
		;
#pragma endregion

}