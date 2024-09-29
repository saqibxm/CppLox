#pragma once

#include <vector>
#include <cctype>
#include <unordered_map>

#include "Token.hpp"
#include "Diagnostics/Diagnostics.hpp"

namespace lox {
	
class Lexer
{
	using RepType = std::string;
	using RepUnit = std::string::value_type;
	// using TokenQueue = std::queue<Token>;
public:
	Lexer();
	Lexer(const std::string &source);
	Lexer& operator=(const std::string&);
	Lexer& operator=(std::string&&) noexcept;
	
	const std::vector<Token>& Scan();
	void ScanToken();
	void Open(const std::string &source);
	void Open(std::string &&source);
	void Reset();

private:
	std::string source;
	std::vector<Token> tokens;
	Diagnostics diagnostics;

	std::size_t start;
	std::size_t current;
	std::size_t line;

	static const std::unordered_map<std::string, TokenType> keywords;

	bool at_end() const { return current >= source.length(); }

	[[maybe_unused]] auto advance() { return source[current++]; }
	void increment() { ++current; }

	auto peek() const { return at_end() ? '\0' : source.at(current); }
	auto peek_next() const { return current + 1 >= source.length() ? '\0' : source[current + 1]; }

	bool match(std::string::value_type);

	// void add_token(TokenType type, Token::LiteralOptional literal = std::nullopt);
	void add_token(TokenType, Token::LiteralOptional);
	void add_token(TokenType type, Literal literal);
	void add_token(TokenType type);

	void identifier();

	void string();

	void number();
};

namespace util {
	inline std::string make_plural(unsigned count, const std::string &str, std::string_view suffix)
	{
		// Thanks C++ Primer
		return (count > 1 ? str + suffix.data() : str);
	}
}
}