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

lox::ExprNode lox::Parser::equality()
{
	ExprNode expr = comparison();

	while (match(TokenType::NOT_EQUAL, TokenType::COMPARE))
	{
		const Token& oper = previous();
		ExprNode right = comparison();
		expr.reset(new Binary(std::move(expr), oper, std::move(right)));
	}

	return expr;
}

lox::ExprNode lox::Parser::comparison()
{
	ExprNode expr = term();

	while (match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL))
	{
		const Token &oper = previous();
		ExprNode right = term();
		expr.reset(new Binary(std::move(expr), oper, std::move(right)));
	}
	return expr;
}

lox::ExprNode lox::Parser::term()
{
	ExprNode expr = factor();

	while(match(TokenType::MINUS, TokenType::PLUS))
	{
		const auto &oper = previous();
		ExprNode right = factor();
		expr.reset(new Binary(std::move(expr), oper, std::move(right)));
	}
	return expr;
}

lox::ExprNode lox::Parser::factor()
{
	ExprNode expr = unary();

	while (match(TokenType::SLASH, TokenType::ASTERISK))
	{
		const auto &oper = previous();
		ExprNode right = factor();
		expr.reset(new Binary(std::move(expr), oper, std::move(right)));
	}
	return expr;
}

lox::ExprNode lox::Parser::unary()
{
	if (match(TokenType::NOT, TokenType::MINUS))
	{
		const auto &oper = previous();
		ExprNode right = unary();
		return ExprNode(new Unary(std::move(right), oper));
	}

	return primary();
}

lox::ExprNode lox::Parser::primary()
{
	if (match(TokenType::TRUE)) return ExprNode(new Value(true));
	if (match(TokenType::FALSE)) return ExprNode(new Value(false));
	if (match(TokenType::NUL)) return ExprNode(new Value(Literal::null));

	if (match(TokenType::STRING, TokenType::NUMBER)) return ExprNode(new Value(previous().literal));
	if (match(TokenType::LPAREN))
	{
		ExprNode node = expression();
		consume(TokenType::RPAREN, "Expected ')' after expression.");
		return ExprNode(new Grouping(std::move(node)));
	}
	throw error(peek(), "Expected an expression!");
	return nullptr;
}
