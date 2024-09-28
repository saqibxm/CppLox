#pragma once

#include "Common.h"

namespace lox {
	inline bool had_error = false;

	void Run(const std::string&);
	void RunFile(const fs::path&);
	void RunPrompt();
}