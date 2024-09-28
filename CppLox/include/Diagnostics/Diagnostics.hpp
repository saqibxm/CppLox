#pragma once

#include <string>
#include <string_view>

#include "Token.hpp"

namespace lox
{

class Diagnostics
{
public:
	enum Type { WARNING, ERROR };
	explicit Diagnostics(const std::string& = std::string{});
	Diagnostics& operator=(const std::string&);

	void Report(Type, std::size_t, const std::string&);

	void Error(std::size_t, std::size_t, std::string_view); // line number and starting position
	void Error(std::size_t, std::string_view); // line number : only this should work given no source
	void Error(Token, std::string_view);

	void Warn(std::size_t, std::size_t, std::string_view); // line number and starting position
	void Warn(std::size_t, std::string_view);
	void Warn(Token, std::string_view);

	void Reset();

private:
	std::string_view source;
	bool hasSource;
	bool hasError;
	std::size_t errorCount;

	[[deprectated("Use the ostream version!")]]
	static std::string decorate(std::string_view msg, std::size_t pos, std::size_t len = 1); // highlight the specified token
	static std::ostream& decorate(std::ostream&, std::string_view, std::size_t, std::size_t, std::size_t = 1);
};

}