#pragma once

#include <unordered_map>

#include "AST.hpp"

#include "Diagnostic.hpp"
#include "EnumHash.hpp"
#include "Lexer.hpp"
#include "fmt/format.h"
#include "status_value.hpp"

namespace pants {
class Parser {
  public:
    struct ParseError{
        ParseError(){}
    };

    Parser(Lexer &lexer);
    AST Parse();

    char getChar();
    void ExpectToken(Token::Kind k);

    const std::vector<Diagnostic> &diags() const { return m_diags; }

    template <typename... Ts>
    void IssueDiagnostic(Token tok, const std::string &fmt_str,
                         const Ts &... ts) {
        m_diags.emplace_back(tok.Row(), tok.Col(), fmt::format(fmt_str, ts...));
        throw ParseError{};
    }

  private:
    std::vector<ASTNodeUP> ParseTopLevelDecls();
    ASTNodeUP ParseFunc();
    VarDeclUP ParseVarDecl();
    ASTNodeUP ParseFor();
    ASTNodeUP ParseClass();
    ASTNodeUP ParseWhile();
    ASTNodeUP ParseIf();
    ASTNodeUP ParseReturn();
    ASTNodeUP ParseStatement();
    ExprUP ParseSubExpression(int right_binding_power = 0);
    ASTNodeUP ParseExpression();
    Type ParseType();
    Id ParseId();
    std::vector<ASTNodeUP> ParseScopeBody();

    ExprUP UnaryAction(Token tok);
    ExprUP LeftAction(Token tok, ExprUP left);

    int GetLeftBindingPower(Token tok);

    bool IsType(Token tok);
    bool IsBinOp(Token tok);

    Token Lex();

    Lexer &m_lexer;
    std::vector<Diagnostic> m_diags;
};

class UnimplementedException {};
}
