#pragma once

#include <string>
#include <string_view>

#include "Token.hpp"

// Module subject to strong refactoring!
// Whole interface can change to shift to positional diagnostics instead of line numbers, and the pos be
// then used to calculated the line on which the error appears, but then the problem is the absence of source.

namespace lox
{
	struct RuntimeError;
	/*
	struct PositionInfo
	{
		std::size_t line;
		std::size_t pos;
		std::size_t len;
	};
	*/

	// TODO: pass a boolean parameter that decides whether the error count persists (not when running line by line)
	// Or simply add a member function to turn it off
	// Also make it a singleton or use dependency injection

class Diagnostics
{
	using PositionInfo = std::tuple<std::size_t, std::size_t, std::size_t>; // line, position, length
public:
	enum Type { WARNING, ERROR };
 
 // Diagnostics() = default;
	explicit Diagnostics(const std::string& = std::string{}); // susceptible to change
	Diagnostics(std::string&&) = delete;

	Diagnostics& operator=(const std::string&);
	Diagnostics operator=(std::string&&) = delete;

	void SetSource(const std::string& src) {
		source = src;
		hasSource = !source.empty();
	}
	void SetSource(std::string&&) = delete;

	void Report(Type, std::size_t, const std::string&);
	void Report(Type, std::size_t, std::string_view, const std::string&); // line where and what variation

	void Error(std::size_t line, std::size_t pos, std::string_view msg); // line number and starting position
	// void Error(std::string_view msg, std::size_t line, std::size_t pos);
	void Error(std::size_t line, std::string_view msg); // line number : only this should work given no source
	void Error(Token, const std::string&);

	void RuntimeError(const lox::RuntimeError&);
	void RuntimeError(const Literal&, const std::string&);

	void Warn(std::size_t line, std::size_t pos, std::string_view msg); // line number and starting position
	// void Warn(std::string_view msg, std::size_t line, std::size_t pos);
	void Warn(std::size_t line, std::string_view msg);
	void Warn(Token, const std::string&);

	void Reset();
	bool HasError() const;
	bool HadRuntimeError() const;
	std::size_t ErrorCount() const;
	std::size_t WarnCount() const;
	std::pair<std::size_t, std::size_t> Count() const;

private:
	std::string_view source;
	bool hasSource;
	bool hasError; // redundant, since count is present
	bool hadRuntimeError;
	std::size_t errorCount, warnCount;

	void intenal_report(std::ostream&, Type, std::string_view, PositionInfo); // unified logic
	[[deprecated("Use the indicate ostream version!")]]
	static std::string decorate(std::string_view msg, std::size_t pos, std::size_t len = 1); // highlight the specified token
	static std::ostream& indicate(std::ostream&, std::string_view, std::size_t, std::size_t, std::size_t = 1);
	static std::size_t count_lines(std::string_view src, std::size_t pos);
};

	struct RuntimeError : public std::runtime_error
	{
		RuntimeError(const Token &tok, const std::string &msg)
			: runtime_error(msg), token(tok)
		{
			;
		}

		Token token;
	};

	extern Diagnostics diagnostics;
}