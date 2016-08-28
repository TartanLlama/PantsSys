#include <iostream>

#include "Parser.hpp"
#include "AST.hpp"

namespace pants {
	Parser::Parser(Lexer& lexer) :m_lexer{ lexer } {

	}

	Lexer::Maybe<Token> Parser::Lex() {
		return m_lexer.Lex();
	}

	Maybe<bool> Parser::ExpectToken(Token::Kind k) {
		auto got = Lex();
		if (!got.status()) return{ ParseStatus::End };

		if (got.value() != k) {
			IssueDiagnostic(got.value(), "Expected '{}', got '{}'.", "kind", got.value().ToString());
			return{ ParseStatus::Continue, false };
		}

		return{ ParseStatus::Continue, true };
	}

	Maybe<ASTNodeUP> Parser::ParseTopLevelDecl() {
		auto tok = Lex();

		if (tok.status() == ParseStatus::End) {
			return{ ParseStatus::End };
		}

		switch (tok.value().Type()) {
		case Token::func_: return ParseFunc();
		case Token::import_:
		case Token::class_:
		case Token::enum_:
		default: 
			IssueDiagnostic(tok.value(), "Unexpected token of type {}", tok.value().ToString());
		}

		return{ ParseStatus::Continue,{} };
	}

	Maybe<ASTNodeUP> Parser::ParseFunc() {
		auto mid = Lex();
		if (!mid.status()) {
			return{ ParseStatus::End };
		}

		if (mid.value() != Token::id_) {
			IssueDiagnostic(mid.value(), "Unexpected token {}", mid.value().ToString());
		}
		
		auto func = std::make_unique<FuncDecl>();
	}

	AST Parser::Parse() {
		auto node = ParseTopLevelDecl();

		return{ std::move(node.value()) };
	}
}
