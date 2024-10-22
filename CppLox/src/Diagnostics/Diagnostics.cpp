#include <iostream>
#include <exception>

#include "Diagnostics/Diagnostics.hpp"
#include "Interpreter/Interpreter.hpp"

using namespace lox;

Diagnostics lox::diagnostics;

Diagnostics::Diagnostics(const std::string &src)
	: source(src), hasSource(!source.empty()), hasError(false), hadRuntimeError(false), errorCount(0)
{
	;
}

Diagnostics& Diagnostics::operator=(const std::string &src)
{
	SetSource(src);
	return *this;
}

void lox::Diagnostics::Report(Type type, std::size_t line, const std::string &msg)
{
	/*if (type == WARNING) Warn(type, line, msg);
	else if (type == ERROR) Error(type, line, msg);
	else throw std::invalid_argument("Unknown Diagnostics Type!");*/
}

void lox::Diagnostics::Error(std::size_t line, std::size_t pos, std::string_view msg)
{
	hasError = true;
	++errorCount;

	intenal_report(std::cerr, Type::ERROR, msg, {line, pos, 1});
}

void lox::Diagnostics::Error(std::size_t line, std::string_view msg)
{
	std::cerr << "[ERROR] at Line " + std::to_string(line + 1) << ' ' << msg << std::endl;
	if (hasSource)
	{
		std::string::size_type marker = 0;
		while (line && (marker = source.find('\n', marker)) != std::string::npos) --line, ++marker;
		std::cerr << '\t' << "Line | " << std::string(source, marker, source.find('\n', marker) - marker)
			<< '\n' << std::endl;
	}
}

void lox::Diagnostics::Error(Token token, const std::string &msg)
{
	/*
	if (token.type == TokenType::EOFILE) {
		Error(token.get_line(), token.get_pos(), " at end " + msg);
	}
	else {
		Error(token.get_line(), token.get_pos(), " at '" + token.lexeme + "' " + msg);
	}
	*/
	++errorCount;
	hasError = true;
	intenal_report(std::cerr, Type::ERROR, msg, { token.get_line(), token.get_pos(), token.lexeme.length() });
}

void lox::Diagnostics::RuntimeError(const lox::RuntimeError &error)
{
	// std::cerr << error.what() << '\n' << "[Line:" << error.token.get_line() << "]\n";
	std::cerr << "A lox Runtime Error has occured!\n";
	Error(error.token, error.what());
	// Todo make the indicator point to the violating operand/literal/token
	if (--errorCount == 0) hasError = false;
	hadRuntimeError = true;
}

void lox::Diagnostics::Warn(std::size_t line, std::size_t pos, std::string_view msg)
{
	intenal_report(std::cerr, Type::WARNING, msg, { line, pos, 1 });
}

void lox::Diagnostics::Warn(std::size_t line, std::string_view msg)
{
	std::cerr << "[WARNING] on Line " + std::to_string(line + 1) << ' ' << msg << std::endl;
	if (hasSource)
	{
		std::string::size_type marker = 0;
		while (line && (marker = source.find('\n', marker)) != std::string::npos) --line, ++marker;
		std::cerr << '\t' << "Line | "
			<< std::string(source, marker, source.find('\n', marker) - marker)
			<< '\n' << std::endl;
	}
}

void lox::Diagnostics::Warn(Token token, const std::string &msg)
{
	intenal_report(std::cerr, Type::WARNING, msg, { token.get_line(), token.get_pos(), token.get_len()});
}

void lox::Diagnostics::intenal_report(std::ostream &os, Type type, std::string_view msg, PositionInfo info)
{
	std::string severity = type == Type::ERROR ? "[ERROR]" : "[WARNING]";

	const auto[line, pos, len] = info;
	std::string report = severity + " @ <" + std::to_string(line + 1) + ','
		+ (pos == -1 ? "?" : std::to_string(pos + 1)) + "> : " + static_cast<std::string>(msg);

	os << report << std::endl;

	if (!hasSource) return;
	std::string::size_type start = 0;
	for (std::string::size_type i = line; i-- && (start = source.find('\n', start)) != std::string::npos; ++start);
	// get to the newline character the appears line'th time, in other words get to the start of the indicated line

	auto current = source.find('\n'); // once there find it's end {to separate the entire line into a string}
	auto lineInfo = "Line " + std::to_string(line + 1) + " | "; // information about the line number
	auto lineStr = std::string(source, start, current - start); // the actual line extracted from source
	auto fault = lineInfo + lineStr; // the combined info and line itself

	os << '\t' << fault << "\n\t";
	indicate(os, fault, lineInfo.length(), pos, len) << '\n' << std::endl;
}

std::string lox::Diagnostics::decorate(std::string_view msg, std::size_t pos, std::size_t len)
{
	std::string ret(msg.length(), '_');
	for (std::size_t i = 0; i < len && i < msg.length(); ++i) ret[pos + i] = '^';
	return ret;
}

std::ostream& lox::Diagnostics::indicate(std::ostream &os, std::string_view msg, std::size_t start, std::size_t pos, std::size_t len)
{
	auto old_state = os.rdstate();
	os.clear();

	std::string::size_type marker = 0;
	constexpr std::string::value_type wspace = ' ', underline = '_', indicator = '^';
	if (pos == -1) goto ret;

	// this whole part needs logic fix-up and heavy refactoring
	while (marker < start) { os.put(wspace); ++marker; }
	while (marker < start + pos) { os.put(len > 1 ? underline : wspace); ++marker;  }

	for(auto i = len; i; --i) os.put(indicator);
	marker += len;
	if (len > 1) {
		while (marker++ < msg.length()) os.put(underline);
	}
	else {
		while (marker <= msg.length() + 1) { os.put('-'); ++marker; }
		os << "<Here>";
	}

	ret:
	os.setstate(old_state);
	return os;
}

std::size_t lox::Diagnostics::count_lines(std::string_view src, std::size_t pos)
{
	return src.empty() ? std::string::npos : std::count(std::next(src.rbegin(), pos), src.rend(), '\n');
}

void lox::Diagnostics::Reset()
{
	hasError = false;
	errorCount = 0;
}

bool lox::Diagnostics::HasError() const
{
	return hasError;
}

bool lox::Diagnostics::HadRuntimeError() const
{
	return hadRuntimeError;
}

std::size_t lox::Diagnostics::ErrorCount() const
{
	return errorCount;
}

std::size_t lox::Diagnostics::WarnCount() const
{
	return warnCount;
}

std::pair<std::size_t, std::size_t> lox::Diagnostics::Count() const
{
	return std::make_pair(ErrorCount(), WarnCount());
}


/*
void lox::Diagnostics::Error(std::size_t line, std::size_t pos, std::string_view msg)
{
	hasError = true;
	++errorCount;

	std::string severity = "[ERROR]";
	std::string report = severity + " @ <" + std::to_string(line + 1) + ','
		+ std::to_string(pos + 1) + "> : " + static_cast<std::string>(msg);

	std::cerr << report << std::endl;

	if (!hasSource) return;
	std::string::size_type start = 0;
	for (std::string::size_type i = line; i-- && (start = source.find('\n', start)) != std::string::npos; ++start);

	auto current = source.find('\n');
	auto lineInfo = "Line " + std::to_string(line + 1) + " | ";
	auto lineStr = std::string(source, start, current - start);
	auto fault = lineInfo + lineStr;

	std::cerr << '\t' << fault << "\n\t";
	indicate(os, fault, lineInfo.length(), pos, len) << '\n' << std::endl;

void RuntimeError(const lox::RuntimeError&);
void lox::Diagnostics::Error(const std::vector<Token> &tokens, const std::string &msg)
{
	auto start = tokens.front().get_pos();
	auto line = tokens.front().get_line();
	auto pos = start;
	auto len = tokens.back().get_pos() + tokens.back().get_len() - start;

	auto &os = std::cerr;
	os << msg << std::endl;
	for (const auto &token : tokens) os << token.lexeme;
	os << std::endl;
	std::string::size_type marker = 0;
	constexpr std::string::value_type wspace = ' ', underline = '_', indicator = '~';

	while (marker < pos) { os.put(wspace); ++marker; }
	while (marker < start + pos) { os.put(len > 1 ? underline : wspace); ++marker; }

	for (auto i = len; i != 0; --i) os.put(indicator);
	marker += len;
		// while (marker++ < msg.length()) os.put(underline);
	os << std::endl;
}
*/