#pragma once

#include <string>
#include <array>

#include "Common.h"
#include "Literal.hpp"

namespace lox {

	// using Literal = std::variant<std::string, double, const char*>;
	enum class TokenType
	{
		// Global
		// UNKNOWN,

		// Character Tokens : Single
		LPAREN, RPAREN, LBRACE, RBRACE, COMMA, DOT,
		MINUS, PLUS, SCOLON, COLON, SLASH, ASTERISK,
		PERCENT, QMARK, AMPERSAND, LINE,

		// Character Tokens : Multiple
		NOT, NOT_EQUAL, EQUAL, COMPARE,
		GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,
		DOUBLE_PLUS, DOUBLE_MINUS, DOUBLE_AMP, DOUBLE_LINE,

		// Literals
		IDENTIFIER, STRING, NUMBER,

		// Keywords
		IF, ELSE, AND, OR, TRUE, FALSE, FOR, WHILE, FN,
		NUL, PRINT, RETURN, VAR, THIS, SUPER, CLASS,
		BREAK, CONTINUE,

		EOFILE, SIZE // conflicts with internal EOF, SIZE for the total numbers of enumerators
	};

	enum class Keywords /* separate enum for keywords */
	{
		IF, ELSE, AND, OR, TRUE, FALSE, FOR, WHILE, FN,
		NUL, PRINT, RETURN, VAR, THIS, SUPER, CLASS
	};
}

namespace lox::util {

	inline std::string to_string(TokenType type)
	{
		static std::array<std::string, static_cast<std::size_t>(TokenType::SIZE)> strings = {
		"LPAREN", "RPAREN", "LBRACE", "RBRACE", "COMMA", "DOT",
		"MINUS", "PLUS", "SCOLON", "COLON", "SLASH", "ASTERISK",
		"PERCENT", "QMARK", "AMPERSAND", "LINE",
		"NOT", "NOT_EQUAL", "EQUAL", "COMPARE",
		"GREATER", "GREATER_EQUAL", "LESS", "LESS_EQUAL",
		"DOUBLE_PLUS", "DOUBLE_MINUS", "DOUBLE_AMP", "DOUBLE_LINE",
		"IDENTIFIER", "STRING", "NUMBER",
		"IF", "ELSE", "AND", "OR", "TRUE", "FALSE", "FOR", "WHILE", "FN",
		"NUL", "PRINT", "RETURN", "VAR", "THIS", "SUPER", "CLASS",
		"BREAK", "CONTINUE",
		"EOF"
		};

		return strings[static_cast<std::size_t>(type)];
	}
}

namespace lox {

	struct Token
	{
		friend class ASTPrinter;
		friend class RPNPrinter;
		using LiteralOptional = std::optional<Object>;
	public:
		/*Token(TokenType t, std::string value, LiteralOptional lit, std::size_t pos)
			: type(t), lexeme(value), literal(std::move(lit)), position(pos)
		{
			;
		}*/
		Token(TokenType t, std::string value, const Object &lit, std::size_t ln, std::size_t pos = -1)
			: type(t), lexeme(value), literal(lit), line(ln), position(pos)
		{
			;
		}

		friend std::ostream& operator<<(std::ostream&, const Token&);
		std::string to_string() const { return lox::util::to_string(type) + ' ' + lexeme; }
		auto get_line() const { return line; }
		auto get_pos() const { return position; }
		auto get_len() const { return lexeme.length(); }
		// bool is_literal() const { return literal; }
	// private:
		TokenType type = TokenType::EOFILE; // could use a separate enumerator for undefined value i.e. UNKNOWN
		std::string lexeme;
		// std::optional<Literal> literal;
		Object literal;
		std::size_t position;/*, length*/ // both the position and length of the lexeme, the position is relative to the start of the source (first character)
		std::size_t line;
	};

	std::ostream& operator<<(std::ostream&, const Token&);
}