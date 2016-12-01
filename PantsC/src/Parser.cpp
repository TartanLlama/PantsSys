#include <iostream>

#include "AST.hpp"
#include "Parser.hpp"

#define UNIMPLEMENTED throw UnimplementedException{};

namespace pants {
Parser::Parser(Lexer &lexer) : m_lexer{lexer} {}

Token Parser::Lex() { return m_lexer.Lex(); }

bool Parser::IsType(Token tok) {
    //TODO implement user-defined types

    switch (tok) {
    case Token::u8_:
    case Token::u16_:
    case Token::u32_:
    case Token::i8_:
    case Token::i16_:
    case Token::i32_:
    case Token::char_:
    case Token::bool_:
    case Token::nil_:
        return true;
    default:
        return false;
    }

    return true;
}

bool Parser::IsBinOp(Token tok) {
    switch (tok.Type()) {
    case Token::add_:
    case Token::min_:
    case Token::div_:
    case Token::mul_:
    case Token::lt_:
    case Token::le_:
    case Token::gt_:
    case Token::eq_:
    case Token::assign_:
    case Token::and_:
    case Token::or_:
        return true;

    default:
        return false;
    }
}

void Parser::ExpectToken(Token::Kind k) {
    auto got = Lex();

    if (got != k) {
        IssueDiagnostic(got, "Expected '{}', got '{}'.", "kind",
                        got.ToString());
    }
}

ASTNodeUP Parser::ParseTopLevelDecl() {
    auto tok = m_lexer.Peek();

    switch (tok.Type()) {
    case Token::func_:
        return ParseFunc();
    case Token::import_:
    case Token::class_:
    case Token::enum_:
    default:
        IssueDiagnostic(tok, "Unexpected token of type {}",
                        tok.ToString());
    }

    return {};
}

Type Parser::ParseType() {
    auto type = Lex();

    if (!IsType(type) && type != Token::id_) {
        IssueDiagnostic(type, "Unexpected token {}",
                        type.ToString());
    }

    return type;
}

ASTNodeUP Parser::ParseVarDecl() { UNIMPLEMENTED }
ASTNodeUP Parser::ParseFor() { UNIMPLEMENTED }
ASTNodeUP Parser::ParseWhile() { UNIMPLEMENTED }
ASTNodeUP Parser::ParseReturn() { UNIMPLEMENTED }

ASTNodeUP Parser::ParseOperand() {
    auto operand = Lex();

    if (operand == Token::int_) {
        return std::make_unique<Int>(operand);
    }

    if (operand == Token::id_) {
        return std::make_unique<Id>(operand);
    }

    IssueDiagnostic(operand, "Unexpected token {}",
                    operand.ToString());
}

ASTNodeUP Parser::ParseBinOpExpr() {
    auto lhs = ParseOperand();

    auto op = Lex();

    if (!IsBinOp(op)) {
        IssueDiagnostic(op, "Unexpected token {}",
                        op.ToString());
    }

    auto rhs = ParseOperand();

    return std::make_unique<BinaryOp>(lhs->Tok(), std::move(lhs), std::move(rhs), op);
}

int Parser::GetLeftBindingPower(Token tok) {
    switch (tok.Type()) {
    case Token::and_:
    case Token::or_:
        return 10;

    case Token::ge_:
    case Token::gt_:
    case Token::lt_:
    case Token::le_:
        return 20;
        
    case Token::min_:
    case Token::add_:
        return 30;
        
    case Token::mul_:
    case Token::div_:
        return 40;
        
    case Token::semi_:
        return 0;
    }
}

ExprUP Parser::UnaryAction(Token tok) {
    switch (tok.Type()) {
    case Token::int_:
        return ExprUP{std::make_unique<Int>(tok)};
    case Token::id_:
        return ExprUP{std::make_unique<Id>(tok)};
    case Token::true_:
    case Token::false_:
        return ExprUP{std::make_unique<Bool>(tok)};
        
    default:
        throw UnimplementedException{};
    }

    return nullptr;
}

ExprUP Parser::LeftAction(Token tok, ExprUP left) {
    switch (tok.Type()) {
    case Token::ge_:
    case Token::gt_:
    case Token::lt_:
    case Token::le_:        
    case Token::and_:
    case Token::or_:        
    case Token::add_:
    case Token::min_:
    case Token::mul_:
    case Token::div_: {
        auto right_s = ParseSubExpression(GetLeftBindingPower(tok));
        return ExprUP{std::make_unique<BinaryOp>(
                                           left->Tok(), std::move(left),
                                           std::move(right_s), tok)};
    }
    default:
        throw UnimplementedException{};
    }

    return nullptr;
}

ExprUP Parser::ParseSubExpression(int right_binding_power) {
    auto tok_s = Lex();
    auto tok = tok_s;

    auto left_s = UnaryAction(tok);
    auto left = std::move(left_s);

    auto next_s = m_lexer.Peek();
    auto next = next_s;

    while (right_binding_power < GetLeftBindingPower(next)) {
        Lex();
        tok = next;

        auto new_left = LeftAction(tok, std::move(left));
        left = std::move(new_left);

        auto new_next = m_lexer.Peek();
        next = new_next;
    }

    return std::move(left);
}

ASTNodeUP Parser::ParseExpression() {
    auto expr = ParseSubExpression();
    ExpectToken(Token::semi_);
    return ASTNodeUP{std::move(expr)};
}

ASTNodeUP Parser::ParseStatement() {
    auto tok = m_lexer.Peek();

    switch (tok.Type()) {
    case Token::for_:
        return ParseFor();
    case Token::while_:
        return ParseWhile();
    case Token::return_:
        return ParseReturn();
    default:
        break;
    }

    if (tok == Token::id_ && IsType(tok)) {
        return ParseVarDecl();
    }

    auto expr = ParseExpression();
    return expr;
}

ASTNodeUP Parser::ParseFunc() {
    auto func_tok = Lex();

    auto id = Lex();
    if (id == Token::eof_) return nullptr;

    if (id != Token::id_) {
        IssueDiagnostic(id, "Unexpected token {}",
                        id.ToString());
    }

    ExpectToken(Token::lsquare_);

    std::vector<VarDeclUP> params{};

    while (true) {
        auto tok = m_lexer.Peek();

        if (tok == Token::rsquare_) {
            break;
        }

        auto type = ParseType();

        auto id = Lex();
        if (id == Token::eof_) return nullptr;

        if (id != Token::id_) {
            IssueDiagnostic(id, "Unexpected token {}",
                            id.ToString());
        }

        params.push_back(std::make_unique<VarDecl>(
            type.Tok(), type, Id{id}));

        if (tok != Token::comma_) {
            IssueDiagnostic(tok, "Unexpected token {}",
                            id.ToString());
        }
    }

    ExpectToken(Token::rsquare_);
    ExpectToken(Token::arrow_);
    auto ret = ParseType();
    ExpectToken(Token::is_);

    std::vector<ASTNodeUP> body{};

    while (true) {
        auto tok = m_lexer.Peek();

        if (tok == Token::end_)
            break;

        auto stmt = ParseStatement();
        body.push_back(std::move(stmt));
    }

    auto func = std::make_unique<FuncDecl>(func_tok, Id{id},
                                           ret, std::move(params),
                                           std::move(body));
    return std::unique_ptr<ASTNode>{std::move(func)};
}

AST Parser::Parse() {
    auto node = ParseTopLevelDecl();

    return {std::move(node)};
}
}
