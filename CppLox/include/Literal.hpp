#pragma once

#include <variant>
#include <optional>
#include <string>
// #include <sstream>

namespace lox {
	using Operand = double;
	template <typename R, typename V>
	R get_type(const V& variant)
	{
		return std::get<R>(variant);
	}

struct Literal : public std::variant<std::monostate, Operand, std::string, std::nullptr_t, bool>
{
	using Base = std::variant<std::monostate, Operand, std::string, std::nullptr_t, bool>;
	// using Base::variant;
	using Base::variant;
	static const Literal None;
	static const inline std::nullptr_t null{};
	// template <typename T, typename...Args>
	// explicit AlternateLiteral(std::in_place_type_t<T> ipt, Args&&...args) : Base(std::move(ipt), std::forward<Args>(args)...) {}

	std::string str() const {
		auto s = std::visit([](const auto &v) -> std::string {
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
			else if constexpr (std::is_same_v<T, std::nullptr_t>)
			{
				return "null";
			}
			else if constexpr (std::is_same_v<T, bool>)
			{
				return v ? "true" : "false";
			}
			else return {};
		}, static_cast<const Base&>(*this));

		return s;
	}

	void set(Operand op);
	void set(std::string_view id);
	void set(std::nullptr_t);
	void set(bool b);
	void reset();
	bool empty() const { return std::holds_alternative<std::monostate>(*this); }

	std::string get_strliteral() const;
	Operand get_number() const;
	bool get_boolean() const;
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

/*
class Literal
{
public:
	using Variant = std::variant<std::monostate, Operand, std::string, std::nullptr_t, bool>; // string for identifier and str literal and double for number
	static const Literal None;
	static const inline std::nullptr_t null{};
	// using Optional = std::optional<Variant>;
	// Literal(std::nullopt_t nopt = null);

	Literal() = default;
	explicit Literal(Operand op);
	Literal(std::string_view str);
	explicit Literal(std::nullptr_t);
	// Literal(bool);
	// Literal(Variant);


	template <typename T, typename...Args>
	explicit Literal(std::in_place_type_t<T>, Args&&...);


	std::string get() const {
		auto str = std::visit([](const auto &v) -> std::string {
		using T = std::decay_t<decltype(v)>;
		if constexpr (std::is_same_v<T, Operand>)
		{
			auto str = std::to_string(v);
			if (std::string::size_type pos = str.find_last_not_of('0');
				pos != std::string::npos/* && str[pos] == '.'/)
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
		else if constexpr (std::is_same_v<T, std::nullptr_t>)
		{
			return "null";
		}
		else if constexpr (std::is_same_v<T, bool>)
		{
			return v ? "true" : "false";
		}
		else return {};
		}, literal);

		return str;
	}

	void set(Operand op);
	void set(std::string_view id);
	void reset();
	bool empty() const { return std::holds_alternative<std::monostate>(literal); }

// private:
	Variant literal;

	std::string get_identifier() const;
	std::string get_strliteral() const;
	Operand get_number() const;
};


template<typename T, typename ...Args>
inline Literal::Literal(std::in_place_type_t<T> ipt, Args &&...args)
	: literal(std::move(ipt), std::forward<Args>(args)...)
{
}
*/