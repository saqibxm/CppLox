#include "Parser/Parser.hpp"

lox::Parser::Parser(const TokenQueue &toks)
	: tokens(toks)
{
}

/*
lox::Parser& lox::Parser::operator=(const TokenQueue &tok)
{
	tokens = tok;
}*/

lox::Expr lox::Parser::conditional()
{
	Expr expr = separation();
	if (match(TokenType::QMARK))
	{
		Expr left = expression();
		const auto &oper = peek(); // get the current token, and check if its a colon
		consume(TokenType::COLON, "Expected ':' for conditional");
		// if (oper.type != TokenType::COLON) throw error(oper, "Expected a colon!");
		Expr right = conditional();
		expr.reset(new Conditional(std::move(expr), std::move(left), std::move(right)));
	}
	return expr;
}

lox::Expr lox::Parser::separation()
{
	Expr expr = equality();

	while (match(TokenType::COMMA))
	{
		const auto &oper = previous(); // get the comma operator's token
		Expr right = equality();
		expr.reset(new Binary(std::move(expr), oper, std::move(right)));
		// TODO decide whether to use a special node for comma.
	}
	return expr;
}

lox::Expr lox::Parser::equality()
{
	Expr expr = comparison();

	while (match(TokenType::NOT_EQUAL, TokenType::COMPARE))
	{
		const Token& oper = previous();
		Expr right = comparison();
		expr.reset(new Binary(std::move(expr), oper, std::move(right)));
	}

	return expr;
}

lox::Expr lox::Parser::comparison()
{
	Expr expr = term();

	while (match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL))
	{
		const Token &oper = previous();
		Expr right = term();
		expr.reset(new Binary(std::move(expr), oper, std::move(right)));
	}
	return expr;
}

lox::Expr lox::Parser::term()
{
	Expr expr = factor();

	while(match(TokenType::MINUS, TokenType::PLUS))
	{
		const auto &oper = previous();
		Expr right = factor();
		expr.reset(new Binary(std::move(expr), oper, std::move(right)));
	}
	return expr;
}

lox::Expr lox::Parser::factor()
{
	Expr expr = unary();

	while (match(TokenType::SLASH, TokenType::ASTERISK))
	{
		const auto &oper = previous();
		Expr right = factor();
		expr.reset(new Binary(std::move(expr), oper, std::move(right)));
	}
	return expr;
}

lox::Expr lox::Parser::unary()
{
	if (match(TokenType::NOT, TokenType::MINUS, TokenType::DOUBLE_MINUS, TokenType::DOUBLE_PLUS))
	{
		const auto &oper = previous();
		Expr right = unary();
		return Expr(new Unary(std::move(right), oper));
	}

	return secondary();
}

lox::Expr lox::Parser::secondary()
{
	Expr expr = primary();
	if (match(TokenType::DOUBLE_MINUS, TokenType::DOUBLE_PLUS))
	{
		const auto &oper = previous();
		return Expr(new Unary(std::move(expr), oper));
	}
	return expr;
}

lox::Expr lox::Parser::primary()
{
	if (match(TokenType::TRUE)) return Expr(new Value(true));
	if (match(TokenType::FALSE)) return Expr(new Value(false));
	if (match(TokenType::NUL)) return Expr(new Value(Literal::null));

	if (match(TokenType::STRING, TokenType::NUMBER)) return Expr(new Value(previous().literal));
	if (match(TokenType::LPAREN))
	{
		Expr node = expression();
		consume(TokenType::RPAREN, "Expected ')' after expression.");
		return Expr(new Grouping(std::move(node)));
	}

	// Error Handling: Stray Binary Operators
	if (auto oper = peek(); match(TokenType::PLUS))
	{
		error(previous(), "Expected an operand to " + oper.lexeme);
		term();
		return nullptr;
	}
	else if (match(TokenType::ASTERISK, TokenType::SLASH))
	{
		error(oper, "Expected an operand to " + oper.lexeme);
		factor();
		return nullptr;
	}
	else if (match(TokenType::NOT_EQUAL, TokenType::COMPARE))
	{
		error(oper, "Expected an operand to " + oper.lexeme);
		equality();
		return nullptr;
	}
	else if (match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL))
	{
		error(oper, "Expected an operand to " + oper.lexeme);
		comparison();
		return nullptr;
	}
		// || oper == TokenType::ASTERISK || oper == TokenType::SLASH);
	throw error(peek(), "Expected an expression!");
	return nullptr;
}
