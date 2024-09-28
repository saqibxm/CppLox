#include <iostream>
#include <cstdlib>
#include <cctype>

#include "Lexer/Lexer.hpp"

const std::unordered_map<std::string, lox::TokenType> lox::Lexer::keywords = {
	{ "if", TokenType::IF },
	{ "else", TokenType::ELSE },
	{ "and", TokenType::AND },
	{ "or", TokenType::OR },
	{ "true", TokenType::TRUE },
	{ "false", TokenType::FALSE },
	{ "for", TokenType::FOR },
	{ "while", TokenType::WHILE },
	{ "fn", TokenType::FN },
	{ "null", TokenType::NUL },
	{ "print", TokenType::PRINT },
	{ "return", TokenType::RETURN },
	{ "var", TokenType::VAR },
	{ "this", TokenType::THIS },
	{ "super", TokenType::SUPER },
	{ "class", TokenType::CLASS }
};

lox::Lexer::Lexer()
	: source(), start(0), current(0), line(0)
{
}

lox::Lexer::Lexer(const std::string & src)
	: source(src), start(0), current(0), line(0), diagnostics(source)
{
}

lox::Lexer & lox::Lexer::operator=(const std::string &src)
{
	Open(src);
	return *this;
}

lox::Lexer & lox::Lexer::operator=(std::string &&src) noexcept
{
	Open(std::move(src));
	return *this;
}

void lox::Lexer::Open(const std::string &src)
{
	source = src;
	diagnostics = source;
}

void lox::Lexer::Open(std::string &&src)
{
	source = std::move(src);
	diagnostics = source;
}

const std::vector<lox::Token>& lox::Lexer::Scan()
{
	while (!at_end())
	{
		start = current; // move the start to the current character being considered : relative to source start
		ScanToken();
	}

	tokens.emplace_back(TokenType::EOFILE, std::string(), std::nullopt, line);
	return tokens;
}

void lox::Lexer::ScanToken()
{
	auto c = advance();

	switch (c)
	{
		// Single Characters
	case '(': add_token(TokenType::LPAREN); break;
	case ')': add_token(TokenType::RPAREN); break;
	case '{': add_token(TokenType::LBRACE); break;
	case '}': add_token(TokenType::RBRACE); break;
	case ',': add_token(TokenType::COMMA);  break;
	case '.': add_token(TokenType::DOT);    break;
	case '-': add_token(TokenType::MINUS);  break;
	case '+': add_token(TokenType::PLUS);   break;
	case ';': add_token(TokenType::SCOLON); break;
	case '*': add_token(TokenType::ASTERISK); break;

	case '/': // comment ought to be ignored
		if (match('/')) {
			while (!at_end() && peek() != '\n') increment();
		}
		else if (match('*')) {
			while (!at_end() && (peek() != '*' && peek_next() != '/'))
			{
				if (peek() == '\n') ++line;
				increment();
			}
			if (at_end())
				diagnostics.Error(line, current, "Unterminated block comment!\n");
			// automatic fall through to the nearest break statement
		}
		else add_token(TokenType::SLASH);
		break;

		// Operators
	case '!': add_token(match('=') ? TokenType::NOT_EQUAL : TokenType::NOT); break;
	case '=': add_token(match('=') ? TokenType::COMPARE : TokenType::EQUAL); break;
	case '<': add_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
	case '>': add_token(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;

	case '\n': ++line; break;

	case '\"': string(); break;

	default:
		// Todo: make a diagnostics system to handle this better
		if (std::isspace(c));
		else if (std::isalpha(c)) {
			identifier();
		}
		else if (std::isdigit(c)) {
			number();
		}
		else {
			diagnostics.Error(line, current - 1, "Unknown Token : " + std::string(1, c) + '\n');
			// current - 1 problem arises from the structure since its advance() on top not peek()
			std::abort(); // remove and place a message on top that checks if diagnostic system ahs reported
			// error and prints their count
		}
		break;
	}
}

void lox::Lexer::Reset()
{
	tokens.clear();

	start = current = line = 0;
}


bool lox::Lexer::match(std::string::value_type expected) {
	if (at_end() || peek() != expected) return false;
	increment();
	return true;
}

void lox::Lexer::add_token(TokenType type, Token::LiteralOptional literal) {
	// can be split into two overloads one that takes Literal (not optional literal) and one that doesnt.
	std::string text = source.substr(start, current - start);
	tokens.emplace_back(type, text, literal, start);
}

void lox::Lexer::add_token(TokenType type, Literal literal)
{
	std::string text = source.substr(start, current - start);
	tokens.emplace_back(type, text, literal, start);
}

void lox::Lexer::add_token(TokenType type)
{
	add_token(type, std::nullopt);
}

void lox::Lexer::identifier()
{
	while (std::isalnum(peek()))
	{
		increment();
	}

	// if (at_end()) return; // to be implemented

	std::string identifier = source.substr(start, current - start);
	if (auto it = keywords.find(identifier); it != keywords.end()) add_token(it->second);
	else add_token(TokenType::IDENTIFIER);
}

void lox::Lexer::string()
{
	RepUnit c;
	while ((c = peek()) != '\"' && !at_end())
	{
		if (c == '\n') ++line;
		increment();
	}

	if (at_end())
	{
		diagnostics.Error(line, current, "Unterminated string literal!\n");
		return;
	}

	std::string str = source.substr(start + 1, current - start - 1); // from start+1(") to one before closing(")
	advance(); // consume the closing quote
	// whether advance is called before or adter add_token has impact on automatic insertion of token lexeme
	add_token(TokenType::STRING, Literal{ str });
}

void lox::Lexer::number()
{
	while (std::isdigit(peek())) increment();
	/*if (match('.') && !std::isdigit(peek()))
		std::cerr << "Trailing decimal point is prohibited\n";
	*/
	if (peek() == '.')
	{
		if (std::isdigit(peek_next()))
		{
			increment();
			while (std::isdigit(peek())) increment();
		}
		else
		{
			diagnostics.Error(line, current, "Trailing decimal point is prohibited!\n");
			return;
		}
	}
	// the reason match method is abolished is because it consumes the character if found, that said { 123.to_string() } would be problematic
	add_token(TokenType::NUMBER, Literal{ std::stod(source.substr(start, current - start)) });
}