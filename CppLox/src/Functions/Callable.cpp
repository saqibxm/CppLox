#include "Functions\Callable.hpp"

lox::NativeFunction::NativeFunction(const std::string &fname)
	: name(fname)
{
}

/*
std::string lox::Callable::to_string() const
{
	static std::ostringstream hexer;
	hexer << std::hex << reinterpret_cast<std::uintptr_t>(this);
	return "<fn:0x" + hexer.str() + '>';
}
*/