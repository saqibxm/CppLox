#include "Parser/Parser.hpp"

using namespace lox::expr;

lox::Parser::Parser(const TokenQueue &toks)
	: tokens(toks), available(true), current(0)
{
}

lox::Parser& lox::Parser::operator=(const TokenQueue &tok)
{
	tokens = const_cast<TokenQueue&>(tok);
	available = true;
	current = 0;
	return *this;
}

lox::Stmt lox::Parser::declaration() try {
	if (match(TokenType::VAR)) return variable_decl();
	if (match(TokenType::LBRACE)) return Stmt(new stmt::Block(block()));
	return statement();
} catch (const ParseError&) {
	synchronize();
	return nullptr;
}  // function try-block

lox::Stmt lox::Parser::variable_decl()
{
	Token name = consume(TokenType::IDENTIFIER, "Expected an identifier in variable declaration.");
	Expr initializer = nullptr;
	if (match(TokenType::EQUAL))
	{
		initializer = expression();
	}
	consume(TokenType::SCOLON, "Expected ';' after variable declaration.");
	return Stmt(new stmt::Var(name, std::move(initializer)));
}

lox::Stmt lox::Parser::statement()
{
	if (match(TokenType::PRINT)) return print_statement();
	return expression_statement();
}

lox::Stmt lox::Parser::print_statement()
{
	Expr expr = expression();
	consume(TokenType::SCOLON, "Expected a semicolon after value.");
	return Stmt(new stmt::Print(std::move(expr)));
}

lox::Stmt lox::Parser::expression_statement()
{
	Expr expr = expression();
	consume(TokenType::SCOLON, "Expected a terminating ';' at the end of statement.");
	return Stmt(new stmt::Expression(std::move(expr)));
}

lox::StatementList lox::Parser::block()
{
	StatementList statements;

	while (!check(TokenType::RBRACE) && !at_end())
	{
		statements.push_back(declaration());
	}
	consume(TokenType::RBRACE, "A corresponding '}' is required to close the block.");
	return statements;
}

lox::Expr lox::Parser::assignment()
{
	Expr exp = conditional();
	if (match(TokenType::EQUAL))
	{
		const Token equals = previous();
		Expr value = assignment(); // chained assignment, a = b = c = 1;

		if (auto ptr = dynamic_cast<expr::Variable*>(exp.get())) // if the last left hand of the equal was a variable
			return Expr(new expr::Assign(ptr->name, std::move(value)));
		else error(equals, "Expected an identifier on left hand of assignment.");
	}
	return exp;
}

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
	if (match(TokenType::IDENTIFIER)) return Expr(new expr::Variable(previous()));
	if (match(TokenType::SCOLON)) return nullptr; // stray semicolon
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

void lox::Parser::synchronize()
{
	advance();
	while (!at_end()) {
		if (previous().type == TokenType::SCOLON) return;

		switch (peek().type) {
		case TokenType::CLASS:
		case TokenType::FN:
		case TokenType::VAR:
		case TokenType::FOR:
		case TokenType::IF:
		case TokenType::WHILE:
		case TokenType::PRINT:
		case TokenType::RETURN:
			return;
		}
		advance();
	}
}