#pragma once

#include <string>
#include <string_view>

#include "Token.hpp"

// Module subject to strong refactoring!
// Whole interface can change to shift to positional diagnostics instead of line numbers, and the pos be
// then used to calculated the line on which the error appears, but then the problem is the absence of source.

namespace lox
{
	/*
	struct PositionInfo
	{
		std::size_t line;
		std::size_t pos;
		std::size_t len;
	};
	*/

class Diagnostics
{
	using PositionInfo = std::tuple<std::size_t, std::size_t, std::size_t>;
public:
	enum Type { WARNING, ERROR };
 
 // Diagnostics() = default;
	explicit Diagnostics(const std::string& = std::string{}); // susceptible to change
 Diagnostics(std::string&&) = delete;

	Diagnostics& operator=(const std::string&);
 Diagnostics operator=(std::string&&) = delete;


	void Report(Type, std::size_t, const std::string&);
	void Report(Type, std::size_t, std::string_view, const std::string&); // line where and what variation

	void Error(std::size_t line, std::size_t pos, std::string_view msg); // line number and starting position
	// void Error(std::string_view msg, std::size_t line, std::size_t pos);
	void Error(std::size_t line, std::string_view msg); // line number : only this should work given no source
	void Error(Token, std::string_view);

	void Warn(std::size_t line, std::size_t pos, std::string_view msg); // line number and starting position
	// void Warn(std::string_view msg, std::size_t line, std::size_t pos);
	void Warn(std::size_t line, std::string_view msg);
	void Warn(Token, std::string_view);

	void Reset();
	bool HasError() const;
	std::size_t ErrorCount() const;
	std::size_t WarnCount() const;
	std::pair<std::size_t, std::size_t> Count() const;

private:
	std::string_view source;
	bool hasSource;
	bool hasError; // redundant, since count is present
	std::size_t errorCount, warnCount;

	void intenal_report(std::ostream&, Type, std::string_view, PositionInfo); // unified logic
	[[deprecated("Use the indicate ostream version!")]]
	static std::string decorate(std::string_view msg, std::size_t pos, std::size_t len = 1); // highlight the specified token
	static std::ostream& indicate(std::ostream&, std::string_view, std::size_t, std::size_t, std::size_t = 1);
	static std::size_t count_lines(std::string_view src, std::size_t pos);
};
}