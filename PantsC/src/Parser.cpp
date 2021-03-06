#include <iostream>
#include <vector>

#include "AST.hpp"
#include "Parser.hpp"

#define UNIMPLEMENTED throw UnimplementedException{};

namespace pants {
Parser::Parser(Lexer &lexer) : m_lexer{lexer} {}

Token Parser::Lex() { return m_lexer.Lex(); }
Token Parser::Peek() { return m_lexer.Peek(); }
Token Parser::PeekMore() { return m_lexer.PeekMore(); }

bool Parser::CouldBeType(Token tok) {
    switch (tok) {
    case Token::u8_:
    case Token::u16_:
    case Token::u32_:
    case Token::i8_:
    case Token::i16_:
    case Token::i32_:
    case Token::char_:
    case Token::bool_:
    case Token::id_:
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
        IssueDiagnostic(got, "Expected '{}', got '{}'.", Token{k,0,0}.ToString(),
                        got.ToString());
    }
}

std::vector<ASTNodeUP> Parser::ParseTopLevelDecls() {
    std::vector<ASTNodeUP> nodes;

    for (auto tok = Peek(); tok != Token::eof_; tok = Peek()) {
        switch (tok.Type()) {
        case Token::func_:
            nodes.push_back(ParseFunc());
            break;
        case Token::import_:
        case Token::class_:
            nodes.push_back(ParseClass());
            break;
        case Token::enum_:
            nodes.push_back(ParseEnum());
            break;
        default:
            IssueDiagnostic(tok, "Unexpected token of type {}",
                            tok.ToString());
        }
    }

    return nodes;
}

Type Parser::ParseType() {
    auto type = Lex();

    if (!CouldBeType(type)) {
        IssueDiagnostic(type, "Unexpected token {}",
                        type.ToString());
    }

    return type;
}

ASTNodeUP Parser::ParseClass() {
    auto class_tok = Lex();
    auto id = ParseId();
    ExpectToken(Token::is_);

    std::vector<VarDeclUP> members{};
    for (auto tok = Peek(); tok != Token::end_; tok = Peek()) {
        members.push_back(ParseVarDecl());
    }

    ExpectToken(Token::end_);

    return std::make_unique<ClassDecl>(class_tok, id, std::move(members));
}

ASTNodeUP Parser::ParseEnum() {
    auto enum_tok = Lex();
    auto id = ParseId();
    ExpectToken(Token::is_);

    std::vector<Id> names{};
    for (auto tok = Peek(); ; tok = Peek()) {
        names.push_back(ParseId());

        if (Peek() == Token::end_) break;

        //allow trailing comma
        if (Peek() == Token::comma_ && PeekMore() == Token::end_) {
            (void)Lex(); break;
        }

        ExpectToken(Token::comma_);
    }

    ExpectToken(Token::end_);

    return std::make_unique<EnumDecl>(enum_tok, id, std::move(names));
}


VarDeclUP Parser::ParseVarDecl() {
    auto type = ParseType();
    auto id = ParseId();

    auto init = ExprUP{nullptr};


    //variable might have an initializer
    auto tok = Peek();
    if (tok == Token::assign_) {
        Lex();
        init = ParseSubExpression();
    }

    ExpectToken(Token::semi_);

    return std::make_unique<VarDecl>(type.Tok(), type, id, std::move(init));
}

ASTNodeUP Parser::ParseFor() { UNIMPLEMENTED }

ASTNodeUP Parser::ParseIf() {
    auto if_tok = Lex();

    auto cond = ParseSubExpression();
    ExpectToken(Token::do_);
    auto if_body = ParseScopeBody();

    std::vector<If::CondBodyPair> conds{};
    conds.emplace_back(std::move(cond),std::move(if_body));

    //parse else if statements
    while (Peek() == Token::else_ && PeekMore() == Token::if_) {
        (void)Lex(); (void)Lex();
        auto else_if_cond = ParseSubExpression();
        ExpectToken(Token::do_);
        auto else_if_body = ParseScopeBody();

        conds.emplace_back(std::move(else_if_cond), std::move(else_if_body));
    }

    //parse else
    std::vector<ASTNodeUP> else_body;
    if (Peek() == Token::else_) {
        (void)Lex();
        ExpectToken(Token::do_);
        else_body = ParseScopeBody();
    }

    return std::make_unique<If>(if_tok, std::move(conds), std::move(else_body));
}

ASTNodeUP Parser::ParseWhile() {
    auto while_tok = Lex();
    auto cond = ParseSubExpression();

    ExpectToken(Token::do_);

    auto body = ParseScopeBody();

    return std::make_unique<While>(while_tok, std::move(cond), std::move(body));
}

ASTNodeUP Parser::ParseReturn() {
    auto ret_tok = Lex();
    auto cond = ParseSubExpression();
    ExpectToken(Token::semi_);
    return std::make_unique<Return>(ret_tok, std::move(cond));
}

int Parser::GetLeftBindingPower(Token tok) {
    switch (tok.Type()) {
    case Token::assign_:
        return 5;

    case Token::or_:
        return 10;

    case Token::and_:
        return 20;

    case Token::ge_:
    case Token::gt_:
    case Token::lt_:
    case Token::le_:
        return 30;

    case Token::min_:
    case Token::add_:
        return 40;

    case Token::mul_:
    case Token::div_:
    case Token::mod_:
        return 50;

    case Token::lparen_:
        return 60;

    case Token::semi_:
    case Token::comma_:
    case Token::rparen_:
        return 0;
        
    default:
        throw std::runtime_error(fmt::format("Unhandled left binding power token {}", tok.ToString()));
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

    case Token::lparen_:
    {
        auto expr = ParseSubExpression();
        ExpectToken(Token::rparen_);
        return expr;
    }

    default:
        throw UnimplementedException{};
    }

    return nullptr;
}

ExprUP Parser::LeftAction(Token tok, ExprUP left) {
    switch (tok.Type()) {
    case Token::assign_:
    case Token::ge_:
    case Token::gt_:
    case Token::lt_:
    case Token::le_:
    case Token::and_:
    case Token::or_:
    case Token::add_:
    case Token::min_:
    case Token::mul_:
    case Token::mod_:
    case Token::div_: {
        auto right_s = ParseSubExpression(GetLeftBindingPower(tok));
        return ExprUP{std::make_unique<BinaryOp>(
                                           left->Tok(), std::move(left),
                                           std::move(right_s), tok)};
    }

    case Token::lparen_:
    {
        std::vector<ExprUP> args;

        if (Peek() != Token::rparen_) {
            while (true) {
                args.push_back(ParseSubExpression());

                auto next_tok = Peek();
                if (next_tok == Token::rparen_) {
                    break;
                }

                ExpectToken(Token::comma_);
            }
        }

        ExpectToken(Token::rparen_);

        return ExprUP{std::make_unique<Call>(left->Tok(), std::move(left), std::move(args))};
    }

    default:
        throw UnimplementedException{};
    }

    return nullptr;
}

ExprUP Parser::ParseSubExpression(int right_binding_power) {
    auto tok = Lex();
    auto left = UnaryAction(tok);
    auto next = Peek();

    while (right_binding_power < GetLeftBindingPower(next)) {
        Lex();
        tok = next;

        auto new_left = LeftAction(tok, std::move(left));
        left = std::move(new_left);

        auto new_next = Peek();
        next = new_next;
    }

    return left;
}

ASTNodeUP Parser::ParseExpression() {
    auto expr = ParseSubExpression();
    ExpectToken(Token::semi_);
    return std::move(expr);
}

ASTNodeUP Parser::ParseStatement() {
    auto tok = Peek();

    switch (tok.Type()) {
    case Token::for_:
        return ParseFor();
    case Token::while_:
        return ParseWhile();
    case Token::return_:
        return ParseReturn();
    case Token::if_:
        return ParseIf();
    default:
        break;
    }

    if (CouldBeType(tok) && PeekMore() == Token::id_) {
        return ParseVarDecl();
    }

    auto expr = ParseExpression();
    return expr;
}

std::vector<ASTNodeUP> Parser::ParseScopeBody() {
    std::vector<ASTNodeUP> body{};

    while (true) {
        auto tok = Peek();

        if (tok == Token::end_)
            break;

        auto stmt = ParseStatement();
        body.push_back(std::move(stmt));
    }

    ExpectToken(Token::end_);

    return body;
}

Id Parser::ParseId() {
    auto id = Lex();

    if (id != Token::id_) {
        IssueDiagnostic(id, "Unexpected token {}",
                        id.ToString());
    }

    return id;
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
        auto tok = Peek();

        if (tok == Token::rsquare_) {
            break;
        }

        auto type = ParseType();

        auto param_id = Lex();
        if (param_id == Token::eof_) return nullptr;

        if (param_id != Token::id_) {
            IssueDiagnostic(param_id, "Unexpected token {}",
                            param_id.ToString());
        }

        params.push_back(std::make_unique<VarDecl>(
            type.Tok(), type, Id{param_id}));

        if (tok != Token::comma_) {
            IssueDiagnostic(tok, "Unexpected token {}",
                            tok.ToString());
        }
    }

    ExpectToken(Token::rsquare_);
    ExpectToken(Token::arrow_);
    auto ret = ParseType();
    ExpectToken(Token::is_);

    auto body = ParseScopeBody();
    auto func = std::make_unique<FuncDecl>(func_tok, Id{id},
                                           ret, std::move(params),
                                           std::move(body));
    return std::unique_ptr<ASTNode>{std::move(func)};
}

AST Parser::Parse() {
    auto nodes = ParseTopLevelDecls();

    return {std::move(nodes)};
}
}
