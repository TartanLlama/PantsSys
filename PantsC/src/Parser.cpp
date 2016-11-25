#include <iostream>

#include "Parser.hpp"
#include "AST.hpp"

#define CHECK_RET(tok) if (!tok.status()) return {ParseStatus::End}

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
        auto tok = m_lexer.Peek();
        CHECK_RET(tok);

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

    Maybe<Id> Parser::ParseType() {
        auto type = Lex();
        CHECK_RET(type);

        if (type.value() != Token::id_) {
            IssueDiagnostic(type.value(), "Unexpected token {}", type.value().ToString());
        }

        return { Id{type.value()} };
    }

    Maybe<ASTNodeUP> Parser::ParseStatement() {
        auto tok = Lex();
        CHECK_RET(tok);

        switch (tok.value().Type()) {
        case Token::for_: return ParseFor();
        case Token::while_: return ParseWhile();
        case Token::return_: return ParseReturn();
        default: break;
        }

        if (IsType(tok)) {
            //parse var decl or static function call
        }
    }

    Maybe<ASTNodeUP> Parser::ParseFunc() {
        auto func_tok = Lex();
        CHECK_RET(func_tok);

        auto id = Lex();
        if (!id.status()) return {ParseStatus::End};

        if (id.value() != Token::id_) {
            IssueDiagnostic(id.value(), "Unexpected token {}", id.value().ToString());
        }

        if (!ExpectToken(Token::lsquare_).status()) return {ParseStatus::End};

        std::vector<VarDeclUP> params{};

        while (true) {
            auto type = ParseType();
            CHECK_RET(type);

            auto id = Lex();
            if (!id.status()) return {ParseStatus::End};

            if (id.value() != Token::id_) {
                IssueDiagnostic(id.value(), "Unexpected token {}", id.value().ToString());
            }

            params.emplace_back(type.value().String().value(), id.value().String().value());

            auto tok = m_lexer.Peek();
            CHECK_RET(tok);

            if (tok.value() == Token::rsquare_) {
                break;
            }
            else if (tok.value() != Token::comma_) {
                IssueDiagnostic(tok.value(), "Unexpected token {}", id.value().ToString());
            }
        }

        CHECK_RET(ExpectToken(Token::rsquare_));
        CHECK_RET(ExpectToken(Token::arrow_));

        auto ret = ParseType();
        CHECK_RET(ret);

        CHECK_RET(ExpectToken(Token::is_));

        std::vector<ASTNodeUP> body{};

        while (true) {
            auto tok = m_lexer.Peek();
            CHECK_RET(tok);

            if (tok.value() == Token::end_) break;

            auto stmt = ParseStatement();
            CHECK_RET(stmt);
            body.push_back(stmt.value());
        }

        auto func = std::make_unique<FuncDecl>(func_tok.value(), Id{id.value()}, ret.value(),
                                               std::move(params), std::move(body));
        return std::unique_ptr<ASTNode>{std::move(func)};
    }

    AST Parser::Parse() {
        auto node = ParseTopLevelDecl();

        return{ std::move(node.value()) };
    }
}
