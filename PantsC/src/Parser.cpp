#include <iostream>

#include "Parser.hpp"
#include "AST.hpp"

namespace pants {
	Parser::Parser(std::istream& is) :m_is{ is }, m_row{ 0 }, m_col{ 0 } {

	}

	Maybe<char> Parser::getChar() {
		char c = m_is.get();
		if (!m_is) return{ ParseStatus::End };

		if (c == '\n') {
			++m_row;
			m_col = 0;
		}
		else {
			++m_col;
		}

		return{ ParseStatus::Continue, c };
	}

	Maybe<bool> Parser::expectChar(char c) {
		auto got = getChar();
		if (!got.status()) return{ ParseStatus::End };

		if (got.value() != c) {
			issueDiagnostic("Expected '{}', got '{}'.", c, got.value());
			return{ ParseStatus::Continue, false };
		}

		return{ ParseStatus::Continue, true };
	}

	Maybe<ASTNodeUP> Parser::parseNode() {
		
		return{ ParseStatus::Continue,{} };
	}

	AST Parser::parse() {
		auto node = parseNode();

		return{ std::move(node.value()) };
	}
}
