#include "Parser/Parser.hpp"

using namespace lox::expr;

/*
#ifndef NDEBUG
// Need a new system to put out all the tokens of a subexpression call into a global object
// which can later be used in diagnostics for proper highlighting and indication
std::vector<lox::Token> lastTokens;
#endif // !NDEBUG
*/

struct ScopedAdvance {
	ScopedAdvance(std::size_t &obj) : target(obj) { ++target; }
	~ScopedAdvance() { --target; }
	std::size_t &target;
};

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
	if (match(TokenType::FN)) return function("function");
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

lox::Stmt lox::Parser::function(const std::string &kind)
{
	// for now we will not allow anonymous functions
	Token name = consume(TokenType::IDENTIFIER, "Expected and identifier for " + kind + " name.");
	consume(TokenType::LPAREN, "Expected '(' after " + kind + " name.");

	TokenList parameters;
	if (check(TokenType::RPAREN)) goto BLOCK;
	do
	{
		if (parameters.size() >= ARGS_UPLIMIT)
			error(peek(), kind + " can't have more than " + std::to_string(ARGS_UPLIMIT) + "parameters.");
		parameters.push_back(consume(TokenType::IDENTIFIER, "Expected an identifier for parameter name."));
	} while (match(TokenType::COMMA));
	/*
	do
	{
		Token param = advance();
	} while (previous().type != TokenType::RPAREN);
	while (!check(TokenType::RPAREN))
	{
		Token param = advance();
		parameters.push_back(std::move(param));
		if (match(TokenType::COMMA)) break;
	}
	*/
	BLOCK:
	consume(TokenType::RPAREN, "Left parentheses ')' required to close parameter list.");
	consume(TokenType::LBRACE, kind + " body starts with a '{'");
	StatementList body = block();
	// stmt::Block body = block();
	return Stmt(new stmt::Function(name, parameters, std::move(body)));
}

lox::Stmt lox::Parser::statement()
{
	if (match(TokenType::IF)) return if_statement();
	if (match(TokenType::WHILE)) return while_loop();
	if (match(TokenType::FOR)) return for_loop();
	if (match(TokenType::PRINT)) return print_statement();
	if (match(TokenType::BREAK, TokenType::CONTINUE)) return loop_control();
	if (match(TokenType::RETURN)) return return_stmt();
	if (match(TokenType::LBRACE)) return Stmt(new stmt::Block(block()));
	return expression_stmt();
}

lox::Stmt lox::Parser::print_statement()
{
	Expr expr = expression();
	consume(TokenType::SCOLON, "Expected a semicolon after value.");
	return Stmt(new stmt::Print(std::move(expr)));
}

lox::Stmt lox::Parser::expression_stmt()
{
	Expr expr = expression();
	consume(TokenType::SCOLON, "Expected a terminating ';' at the end of statement.");
	return Stmt(new stmt::Expression(std::move(expr)));
}

lox::Stmt lox::Parser::if_statement()
{
	consume(TokenType::LPAREN, "Expected opening parentheses '(' after if.");
	Expr condition = expression();
	consume(TokenType::RPAREN, "Closing parentheses ')' required after condition.");
	Stmt then_branch = statement();

	Stmt else_branch = nullptr;
	if (match(TokenType::ELSE))
		else_branch = statement();

	return Stmt(new stmt::IfControl(std::move(condition), std::move(then_branch), std::move(else_branch)));
}

lox::Stmt lox::Parser::while_loop()
{
	ScopedAdvance inc(loop_count);
	consume(TokenType::LPAREN, "Expected opening parentheses '(' after 'while'.");
	Expr condition = expression();
	consume(TokenType::RPAREN, "Closing parentheses ')' expected after condition.");

	Stmt body = statement();
	return Stmt(new stmt::While(std::move(condition), std::move(body)));
}

lox::Stmt lox::Parser::for_loop()
{
	ScopedAdvance inc(loop_count);
	consume(TokenType::LPAREN, "Opening parentheses '(' expected for the 'for' header clause.");

	Stmt initializer;
	if (match(TokenType::SCOLON)) initializer = nullptr;
	else if (match(TokenType::VAR)) initializer = variable_decl();
	else initializer = expression_stmt();
	
	Expr condition;
	if (!check(TokenType::SCOLON)) condition = expression();
	else condition = Expr(new expr::Value(true));

	consume(TokenType::SCOLON, "Expected ';' after loop condition.");

	// Expr modifier = expression(); // won't be desired since there is no handler for RPAREN in primary()
	// this will result in an error ("Expected an expression")
	Expr modifier;
	if (!check(TokenType::RPAREN)) modifier = expression();

	consume(TokenType::RPAREN, "Left parentheses ')' is required after the 'for' clause.");

	Stmt body = statement();
	// Convert to Semantic While Loop
	// Oh std::initializer_list give us convenience of Move semantics
	// oh wise unique_ptr give us non explicit T* constructor

	// lox::StatementList list; // can use push_back or emplace_back
	std::array<lox::Stmt, 2> list;
	auto &[first, second] = list;
	if (modifier)
	{
		// std::array<lox::Stmt, 2> list = { std::move(body), Stmt(new stmt::Expression(std::move(modifier))) }; // works
		first = std::move(body);
		second = Stmt(new stmt::Expression(std::move(modifier)));

		body = Stmt(new stmt::While(
			std::move(condition),
			Stmt(new stmt::Block(StatementList(
				std::make_move_iterator(list.begin()), std::make_move_iterator(list.end()))
				)
			// Stmt(new stmt::Block(std::move(list))
			)
		));
	}
	// list.clear(); // reuse the existing list, cleared up since unique_ptr values are moved

	if (initializer) // if initializer was provided, put it on top
	{
		first = std::move(initializer);
		second = std::move(body);

		// body = Stmt(new stmt::Block(std::move(list)));
		body = Stmt(new stmt::Block(StatementList(
			std::make_move_iterator(list.begin()), std::make_move_iterator(list.end())
		)));
	}

	/** SKETCH
	**/

	return body;
}

lox::Stmt lox::Parser::loop_control()
{
	const auto &oper = previous();

	if (loop_count == 0) error(oper, '\'' + oper.lexeme + "' statement outside the loop.");
	consume(TokenType::SCOLON, "Expected a ';' after " + util::to_string(oper.type) + '.');
	return Stmt(new stmt::LoopControl(oper.type == TokenType::BREAK ? stmt::LoopControl::BREAK : stmt::LoopControl::CONTINUE));
}

lox::Stmt lox::Parser::return_stmt()
{
	Token keyword = previous();
	Expr value = nullptr;

	if (!check(TokenType::SCOLON)) value = expression();
	consume(TokenType::SCOLON, "Expected ';' after return value.");

	return Stmt(new stmt::Return(keyword, std::move(value)));
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

lox::Expr lox::Parser::separation()
{
	Expr expr = assignment();

	while (match(TokenType::COMMA))
	{
		const auto &oper = previous(); // get the comma operator's token
		Expr right = assignment();
		expr.reset(new Binary(std::move(expr), oper, std::move(right)));
		// TODO decide whether to use a special node for comma.
	}
	return expr;
}

lox::Expr lox::Parser::assignment()
{
	Expr exp = logic_or();
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

lox::Expr lox::Parser::logic_or()
{
	Expr expr = logic_and();

	while (match(TokenType::OR, TokenType::DOUBLE_LINE))
	{
		const Token &oper = previous();
		Expr right = logic_and();

		return Expr(new expr::Logical(std::move(expr), oper, std::move(right)));
	}

	return expr;
}

lox::Expr lox::Parser::logic_and()
{
	Expr expr = conditional();

	while (match(TokenType::AND, TokenType::DOUBLE_AMP))
	{
		const auto &oper = previous(); // saved because parsing right side will change marker
		Expr right = conditional();
		return Expr(new expr::Logical(std::move(expr), oper, std::move(right)));
	}
	return expr;
}

lox::Expr lox::Parser::conditional()
{
	Expr expr = equality();
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
	Expr expr = call();
	while (match(TokenType::DOUBLE_MINUS, TokenType::DOUBLE_PLUS))
	{
		const auto &oper = previous();
		return Expr(new Unary(std::move(expr), oper));
	}
	return expr;
}

lox::Expr lox::Parser::call()
{
	Expr expr = primary(); // can't do a+b(2) (call the result of a+b passing in 2), instead will have to do (a+b)(2)

#ifndef NDEBUG
	Token name = previous(); // since no default constructor, just a circumvention
	if (auto var = dynamic_cast<expr::Variable*>(expr.get()))
	name = var->name;
#endif // !NDEBUG

	while (true) // match(LPAREN), this would look more cleaner
	{
		if (match(TokenType::LPAREN)) expr = finish_call(std::move(expr));
		else break;
	}
	if constexpr (debug) {
		if (auto func = dynamic_cast<expr::Call*>(expr.get()))
			func->paren = name;
	// dynamic_cast<expr::Call*>(expr.get())->paren = name;
	}
	return expr;
}

lox::Expr lox::Parser::finish_call(Expr callee)
{
	ExpressionList arguments;

	if (!check(TokenType::RPAREN))
	{
		do {
			if (arguments.size() >= ARGS_UPLIMIT) error(peek(), "Cant have more than " + std::to_string(ARGS_UPLIMIT) + " arguments in a function call.");
			arguments.push_back(assignment()); // arguments.push_back(expression()); changed to disallow comma to be interpreted as an operator
		} while (match(TokenType::COMMA));
	}
	const Token &paren = consume(TokenType::RPAREN, "Expected a closing paren at the end of argument list.");
	return Expr(new expr::Call(std::move(callee), paren, std::move(arguments)));

	/*
	while(!check(RPAREN))
	{
		arguments.push_back(expression());
		advance();
	}
	*/
}

lox::Expr lox::Parser::primary()
{
	//if (check(TokenType::SCOLON))
	//{
	//	int count = 0;
	//	for (auto i = current; current < tokens.size(); ++i)
	//	{
	//		if (tokens[i].type == TokenType::SCOLON) ++i;
	//		else break;
	//	}
	//	while (count > 1) advance(), --count; // stray semicolon
	//	return nullptr;
	//}

	/*
	#ifndef NDEBUG
	lastTokens.push_back(peek());
	#endif // !NDEBUG
	*/

	if (match(TokenType::TRUE)) return Expr(new Value(true));
	if (match(TokenType::FALSE)) return Expr(new Value(false));
	if (match(TokenType::NUL)) return Expr(new Value(lox::null));

	if (match(TokenType::STRING, TokenType::NUMBER)) return Expr(new Value(previous().literal));
	if (match(TokenType::IDENTIFIER)) return Expr(new expr::Variable(previous()));
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