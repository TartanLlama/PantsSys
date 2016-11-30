#pragma once

#include <unordered_map>

#include "AST.hpp"

#include "Diagnostic.hpp"
#include "EnumHash.hpp"
#include "Lexer.hpp"
#include "fmt/format.h"
#include "status_value.hpp"

namespace pants {
class ParseStatus {
public:
  enum Status { End, Continue };
  ParseStatus(Status s) : m_status{s} {}
  ParseStatus(const ParseStatus &rhs) : m_status{Status{rhs}} {}
  operator bool() const { return m_status != End; }
  operator Status() const { return m_status; }
  ParseStatus &operator=(Status s) {
    m_status = s;
    return *this;
  }

private:
  Status m_status;
};

template <typename T> using Maybe = status_value<ParseStatus, T>;

class Parser {
public:
  Parser(Lexer &lexer);
  AST Parse();

  Maybe<char> getChar();
  Maybe<bool> ExpectToken(Token::Kind k);

  const std::vector<Diagnostic> &diags() const { return m_diags; }

  template <typename... Ts>
  void IssueDiagnostic(Token tok, const std::string &fmt_str,
                       const Ts &... ts) {
    m_diags.emplace_back(tok.Row(), tok.Col(), fmt::format(fmt_str, ts...));
  }

private:
  Maybe<ASTNodeUP> ParseTopLevelDecl();
  Maybe<ASTNodeUP> ParseFunc();
  Maybe<ASTNodeUP> ParseVarDecl();
  Maybe<ASTNodeUP> ParseFor();
  Maybe<ASTNodeUP> ParseWhile();
  Maybe<ASTNodeUP> ParseReturn();
  Maybe<ASTNodeUP> ParseStatement();
  Maybe<ExprUP> ParseSubExpression(int right_binding_power=0);
  Maybe<ASTNodeUP> ParseExpression();
  Maybe<ASTNodeUP> ParseBinOpExpr();
  Maybe<ASTNodeUP> ParseOperand();
  Maybe<Id> ParseType();

  Maybe<ExprUP> UnaryAction(Token tok);
  Maybe<ExprUP> LeftAction(Token tok, ExprUP left);

  int GetLeftBindingPower(Token tok);

  bool IsType(Token tok);
  bool IsBinOp(Token tok);

  Lexer::Maybe<Token> Lex();

  Lexer &m_lexer;
  std::vector<Diagnostic> m_diags;
};

class UnimplementedException {};
}
