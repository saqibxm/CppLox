#pragma once

#include <variant>
#include <optional>
#include <string>
// #include <sstream>

namespace lox {

	template <typename R, typename V>
	R get_type(const V& variant)
	{
		return std::get<R>(variant);
	}

class Literal
{
	using Operand = double;
public:
	using Variant = std::variant<std::monostate, Operand, std::string>; // string for identifier and str literal and double for number
	static const Literal None;
	static const inline std::nullptr_t null{};
	// using Optional = std::optional<Variant>;

	Literal() = default;
	// Literal(std::nullopt_t nopt = null);
	Literal(Operand op);
	Literal(std::string_view str);

	std::string get() const {
		auto str = std::visit([](const auto &v) -> std::string {
		using T = std::decay_t<decltype(v)>;
		if constexpr (std::is_same_v<T, Operand>)
		{
			auto str = std::to_string(v);
			if (std::string::size_type pos = str.find_last_not_of('0');
				pos != std::string::npos/* && str[pos] == '.'*/)
			{
				if (str[pos] != '.') ++pos;
				str.erase(pos, str.length() - pos);
			}
			return str;
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			return v;
		}
		else return {};
		}, literal);

		return str;
	}

	void set(Operand op);
	void set(std::string_view id);
	void reset();
	bool empty() const { return std::holds_alternative<std::monostate>(literal); }

private:
	Variant literal;

	std::string get_identifier() const;
	std::string get_strliteral() const;
	Operand get_number() const;
};

}

/*
if constexpr (std::is_same_v<T, std::monostate>) return {};
else {
	std::ostringstream strm;
	strm << std::noshowpoint << v;
	return strm.str();
}
*/