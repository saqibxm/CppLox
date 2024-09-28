#pragma once

#include <variant>
#include <optional>
#include <string>

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
	using Variant = std::variant<std::string, double>; // string for identifier and str literal and double for number
	// using Optional = std::optional<Variant>;

	Literal(Operand op);
	Literal(std::string_view str);

	std::string get() const {
		auto str = std::visit([](const auto &v) {
			using T = std::decay_t<decltype(v)>;
			if constexpr (std::is_same_v<T, Operand>)
			{
				return std::to_string(v);
			}
			else if constexpr (std::is_same_v<T, std::string>)
			{
				return v;
			}
			else return v;
		}, literal);

		return str;
	}

	void set(Operand op);
	void set(std::string_view str);

private:
	Variant literal;

	std::string get_identifier() const;
	std::string get_strliteral() const;
	Operand get_number() const;
};

}