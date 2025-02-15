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
program -> declaration* EOF ;
declaration -> funcDecl | varDecl | statement ;
funcDecl -> "fn" function ;
function -> IDENTIFIER? "(" parameters? ")" block ;
parameters -> IDENTIFIER ( "," IDENTIFIER )* ;
varDecl -> IDENTIFIER ( "=" initializer )? ";" ;
statement -> expressionStmt| printStmt | ifStmt | whileStmt | forStmt | controlStmt | returnStmt | block ;
block -> "{" declaration* "}" ;
expressionStmt -> expression ";" ;
printStmt -> "print" expressionStmt ;
ifStmt -> "if" "(" expression ")" statement ( "else" statement )? ;
whileStmt -> "while" "(" expression ")" statement ; // can make the statement optional
forStmt -> "for" "(" ( varDecl | exprStmt | ";" ) expression? ";" expression? ")" statement ;
// ( varDecl | exprStmt | ";" ) because varDecl or exprStmt already consume a semicolon
controlStmt -> "continue" | "break" ;
returnStmt -> "return" expression? ";" ;

expression -> conditional ;
assignment -> IDENTIFIER "=" expression
	| logic_or ;
logic_or -> logic_and ( "or" | "||" logic_and )* ;
logic_and -> conditional ( "and" | "&&" conditional ) ;
conditional -> separation ( "?" expression : conditional )? ; // refer to cppref C conditonal operator
separation -> equality ( "," equality )* ;
equality -> comparison ( ( "!=" | "==" ) comparison )* ;
comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term -> factor ( ( "-" | "+" ) factor )* ;
factor -> unary ( ( "/" | "*" ) unary )* ;
unary -> ( "!" | "-" | "--" | "++") unary
	| postfix ;
postfix -> call ( "++" | "--" )? ; // prefix has same precedence as unary while postfix has highers
call -> primary ( "(" arguments? ")" )* ; // chained calls supported i.e callback()()()
arguments -> expression ( "," expression )* ;
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
		inline static constexpr unsigned ARGS_UPLIMIT = 255;
	public:
		Parser() : available(false) {}
		Parser(const TokenQueue&);
		Parser& operator=(const TokenQueue&);

		StatementList Parse() {
			if (!available) throw std::invalid_argument("No tokens to parse.");

			StatementList statements; statements.reserve(repl ? 2 : 100);
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
		std::size_t loop_count = 0;

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
		Stmt function(const std::string&);
		Stmt statement();
		Stmt print_statement();
		Stmt expression_stmt();
		Stmt if_statement();
		Stmt while_loop();
		Stmt for_loop();
		Stmt loop_control();
		Stmt return_stmt();
		StatementList block();

		Expr expression() {
			return assignment();
		}
		Expr separation();
		Expr assignment();
		Expr logic_or();
		Expr logic_and();
		Expr conditional();
		Expr equality();
		Expr comparison();
		Expr term();
		Expr factor();
		Expr unary();
		Expr secondary();
		Expr call();
		Expr finish_call(Expr callee);
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