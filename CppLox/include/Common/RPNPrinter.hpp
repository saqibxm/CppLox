#pragma once

#include "Visitor.hpp"

namespace lox {
	class RPNPrinter : public expr::Visitor
	{
	public:
		std::string print(const expr::Expression&);
		std::any visit(const expr::Binary&) override;
		std::any visit(const expr::Unary&) override;
		std::any visit(const expr::Conditional&) override;
		std::any visit(const expr::Grouping&) override;
		std::any visit(const expr::Operator&) override;
		std::any visit(const expr::Value&) override;

	private:
		std::string stringify(const expr::Expression &);
	};
}