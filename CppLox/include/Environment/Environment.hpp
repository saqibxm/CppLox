#pragma once

#include <unordered_map>
#include <memory>

#include "Common.h"
#include "Literal.hpp" // Object
#include "Token.hpp"

#include "Diagnostics/Diagnostics.hpp" // RuntimeError

namespace lox
{
	class Environment : public std::enable_shared_from_this<Environment>
	{
		using Storage = std::unordered_map<std::string, Object>;
	public:
		using Ptr = std::shared_ptr<Environment>;
		Environment() : enclosing(nullptr) {}
		Environment(Ptr outer) : enclosing(outer) {}

		void define(const std::string &name, const Object &init);

		const Object& retrieve(const Token &name) const;
		Object retrieve(const Storage::key_type &name);

		void assign(const Token &name, const Object &val);
		void assign(const Storage::key_type &name, const Object &val);

		void reserve(Storage::size_type n);

	private:
		Storage values;
		Ptr enclosing; // the environment scope that encloses this one
		// cant be a const pointer since that disables copy assignments

		const Object& get(const Token &name) const;
		void set(const Token &name, const Object &val);

		// std::optional<Object> get(const std::string &name) const;
		// void set(const std::string &name);
	};
}

/*
Object get(std::string_view name);
void set(std::string_view name);
*/