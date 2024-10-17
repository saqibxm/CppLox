#pragma once

#include <queue>
#include <vector>

#include "Common.h"
#include "Token.hpp"
#include "Common/Expression.hpp"
#include "Common/Statement.hpp"
#include "Diagnostics/Diagnostics.hpp"

// Follows this grammar
// Conditional operator: left most side has highest precedence, middle has lowest.
/*
expression -> conditional ;
assignment -> IDENTIFIER "=" expression
	| conditional ;
conditional -> separation ( "?" expression : conditional )? ; // refer to cppref C conditonal operator
separation -> equality ( "," equality )* ;
equality -> comparison ( ( "!=" | "==" ) comparison )* ;
comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term -> factor ( ( "-" | "+" ) factor )* ;
factor -> unary ( ( "/" | "*" ) unary )* ;
unary -> ( "!" | "-" | "--" | "++") unary
	| postfix ;
postfix -> primary ( "++" | "--" )? ; // prefix has same precedence as unary while postfix has higher
primary -> NUMBER | STRING | IDENTIFIER | "true" | "false" | "nil" ;
	| "(" expression ")" ;
*/

namespace lox {

	struct ParseError : public std::runtime_error
	{
		using std::runtime_error::runtime_error;
	};

	class Parser
	{
		// using TokenQueue = std::queue <Token, std::vector<Token>>;
		using TokenQueue = std::vector<Token>;
	public:
		Parser() : available(false) {}
		Parser(const TokenQueue&);
		Parser& operator=(const TokenQueue&);

		StatementList Parse() {
			if (!available) throw std::invalid_argument("No tokens to parse.");

			StatementList statements; statements.reserve(100);
			while (!at_end()) statements.push_back(declaration());
			return statements;
			/*
			try {
				return expression();
			}
			catch (const ParseError &) {
				return nullptr;
			}*/
		}

	private:
		TokenQueue tokens;
		TokenQueue::size_type current = 0;

		bool available;

		template <typename...Args>
		bool match(Args...);
		bool at_end() const;
		const Token& peek() const;

		bool check(TokenType) const;
		const Token& advance();
		const Token& previous() const;

		Token consume(TokenType, const std::string&);
		ParseError error(Token, const std::string&);
		void synchronize();


		Stmt declaration();
		Stmt variable_decl();
		Stmt statement();
		Stmt print_statement();
		Stmt expression_statement();
		StatementList block();

		Expr expression() {
			return assignment();
		}
		Expr assignment();
		Expr conditional();
		Expr separation();
		Expr equality();
		Expr comparison();
		Expr term();
		Expr factor();
		Expr unary();
		Expr secondary();
		Expr primary();
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

	inline Token Parser::consume(TokenType type, const std::string &msg)
	{
		if (check(type)) advance();
		else throw error(peek(), msg);

		return previous();
	}

	inline ParseError Parser::error(Token token, const std::string &msg)
	{
		diagnostics.Error(token, msg);
		return ParseError("Encountered Parse Error");
	}
}