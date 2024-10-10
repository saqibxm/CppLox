#pragma once

#include <queue>
#include <vector>

#include "Common.h"
#include "Token.hpp"
#include "Expression.hpp"
#include "Diagnostics/Diagnostics.hpp"

// Follows this grammar
/*
expression -> equality ;
equality -> comparison ( ( "!=" | "==" ) comparison )* ;
comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term -> factor ( ( "-" | "+" ) factor )* ;
factor -> unary ( ( "/" | "*" ) unary )* ;
unary -> ( "!" | "-" ) unary
 | primary ;
primary -> NUMBER | STRING | "true" | "false" | "nil"
 | "(" expression ")" ;
*/

namespace lox {

	class Parser
	{
		// using TokenQueue = std::queue <Token, std::vector<Token>>;
		using TokenQueue = std::vector<Token>;
	public:
		Parser(const TokenQueue&);

		ExprNode expression() {
			return equality();
		}

	private:
		const TokenQueue tokens;
		TokenQueue::size_type current = 0;

		template <typename...Args>
		bool match(Args...);
		bool at_end() const;
		const Token& peek() const;

		bool check(TokenType) const;
		const Token& advance();
		const Token& previous() const;

		void consume(TokenType, const std::string&);

		ExprNode equality();
		ExprNode comparison();
		ExprNode term();
		ExprNode factor();
		ExprNode unary();
		ExprNode primary();
	};

	template<typename ...Args>
	inline bool Parser::match(Args...types)
	{
		// needs optimisations
		static_assert((std::is_same_v<Args, TokenType> && ...), "Only TokenType is allowed!");
		bool ret = (... || check(types));
		if (ret) advance();
		return ret;
	}

	inline bool lox::Parser::at_end() const
	{
		return peek().type == TokenType::EOFILE;
	}

	inline const Token& lox::Parser::peek() const
	{
		return tokens[current];
	}

	inline bool lox::Parser::check(TokenType type) const
	{
		if (at_end()) return false;
		return peek().type == type;
	}

	inline const Token& lox::Parser::advance()
	{
		// if (at_end()) throw std::out_of_range("Index already at end!");
		if (!at_end()) ++current;
		return previous();
	}

	inline const Token & lox::Parser::previous() const
	{
		return tokens[current - 1];
	}

	inline void Parser::consume(TokenType type, const std::string &msg)
	{
		if (match(type)) return;
		else throw std::invalid_argument(msg);
	}
}