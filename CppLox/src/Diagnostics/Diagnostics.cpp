#include <iostream>
#include <exception>

#include "Diagnostics/Diagnostics.hpp"

using namespace lox;

Diagnostics::Diagnostics(const std::string &src)
	: source(src), hasSource(!source.empty()), hasError(false), errorCount(0)
{
	;
}

Diagnostics& Diagnostics::operator=(const std::string &src)
{
	source = src;
	hasSource = !source.empty();
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
	std::cerr /*<< '\t'*/ << fault << std::endl;

	// decorated line
	/*
	for (auto i = 0U; i < lineInfo.length(); ++i) std::cerr.put(' ');
	std::cerr << decorate(lineStr, pos);*/
	// std::cerr << '\t';
	indicate(std::cerr, fault, lineInfo.length(), pos) << std::endl;
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

	// this whole part needs logic fix-up
	while (marker < start) { os.put(wspace); ++marker; }
	while (marker++ < start + pos) os.put(len > 1 ? underline : wspace);

	marker += len;

	if (len > 1) {
		while (len--) os.put(indicator);
		while (marker++ < msg.length()) os.put(underline);
	}
	else os << "^-- Here";

	os.setstate(old_state);
	return os;
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

std::size_t lox::Diagnostics::Count() const
{
	return errorCount;
}
