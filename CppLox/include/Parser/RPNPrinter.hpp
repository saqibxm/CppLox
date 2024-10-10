#pragma once

#include "Visitor.hpp"

namespace lox {
	class RPNPrinter : public ExprVisitor
	{
	public:
		std::string print(const Expression&);
		std::any visit(const Binary&) override;
		std::any visit(const Unary&) override;
		std::any visit(const Grouping&) override;
		std::any visit(const Operator&) override;
		std::any visit(const Value&) override;

	private:
		std::string stringify(const Expression &);
	};
}