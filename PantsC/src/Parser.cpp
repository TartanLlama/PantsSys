#include <iostream>

#include "AST.hpp"
#include "Parser.hpp"

#define CHECK_RET(tok)                                                         \
  if (!tok.status())                                                           \
    return { ParseStatus::End }
#define UNIMPLEMENTED throw UnimplementedException{};

namespace pants {
Parser::Parser(Lexer &lexer) : m_lexer{lexer} {}

Lexer::Maybe<Token> Parser::Lex() { return m_lexer.Lex(); }

bool Parser::IsType(Token tok) { UNIMPLEMENTED }
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

Maybe<bool> Parser::ExpectToken(Token::Kind k) {
  auto got = Lex();
  if (!got.status())
    return {ParseStatus::End};

  if (got.value() != k) {
    IssueDiagnostic(got.value(), "Expected '{}', got '{}'.", "kind",
                    got.value().ToString());
    return {ParseStatus::Continue, false};
  }

  return {ParseStatus::Continue, true};
}

Maybe<ASTNodeUP> Parser::ParseTopLevelDecl() {
  auto tok = m_lexer.Peek();
  CHECK_RET(tok);

  switch (tok.value().Type()) {
  case Token::func_:
    return ParseFunc();
  case Token::import_:
  case Token::class_:
  case Token::enum_:
  default:
    IssueDiagnostic(tok.value(), "Unexpected token of type {}",
                    tok.value().ToString());
  }

  return {ParseStatus::Continue, {}};
}

Maybe<Id> Parser::ParseType() {
  auto type = Lex();
  CHECK_RET(type);

  if (type.value() != Token::id_) {
    IssueDiagnostic(type.value(), "Unexpected token {}",
                    type.value().ToString());
  }

  return {ParseStatus::Continue, type.value()};
}

Maybe<ASTNodeUP> Parser::ParseVarDecl() { UNIMPLEMENTED }
Maybe<ASTNodeUP> Parser::ParseFor() { UNIMPLEMENTED }
Maybe<ASTNodeUP> Parser::ParseWhile() { UNIMPLEMENTED }
Maybe<ASTNodeUP> Parser::ParseReturn() { UNIMPLEMENTED }

Maybe<ASTNodeUP> Parser::ParseOperand() {
  auto operand = Lex();
  CHECK_RET(operand);

  if (operand.value() == Token::int_) {
    return {ParseStatus::Continue, std::make_unique<Int>(operand.value())};
  }

  if (operand.value() == Token::id_) {
    return {ParseStatus::Continue, std::make_unique<Id>(operand.value())};
  }

  IssueDiagnostic(operand.value(), "Unexpected token {}",
                  operand.value().ToString());
}

Maybe<ASTNodeUP> Parser::ParseBinOpExpr() {
  auto lhs = ParseOperand();
  CHECK_RET(lhs);

  auto op = Lex();
  CHECK_RET(op);

  if (!IsBinOp(op.value())) {
    IssueDiagnostic(op.value(), "Unexpected token {}", op.value().ToString());
  }

  auto rhs = ParseOperand();
  CHECK_RET(rhs);

  return {ParseStatus::Continue,
          std::make_unique<BinaryOp>(lhs.value()->Tok(), std::move(lhs.value()),
                                     std::move(rhs.value()), op.value())};
}

Maybe<ASTNodeUP> Parser::ParseStatement() {
  auto tok = m_lexer.Peek();
  CHECK_RET(tok);

  switch (tok.value().Type()) {
  case Token::for_:
    return ParseFor();
  case Token::while_:
    return ParseWhile();
  case Token::return_:
    return ParseReturn();
  default:
    break;
  }

  if (tok.value() == Token::id_ && IsType(tok.value())) {
    return ParseVarDecl();
  }

  tok = m_lexer.Lex();
  CHECK_RET(tok);

  auto expr = ParseBinOpExpr();

  CHECK_RET(ExpectToken(Token::semi_));

  return expr;
}

Maybe<ASTNodeUP> Parser::ParseFunc() {
  auto func_tok = Lex();
  CHECK_RET(func_tok);

  auto id = Lex();
  if (!id.status())
    return {ParseStatus::End};

  if (id.value() != Token::id_) {
    IssueDiagnostic(id.value(), "Unexpected token {}", id.value().ToString());
  }

  if (!ExpectToken(Token::lsquare_).status())
    return {ParseStatus::End};

  std::vector<VarDeclUP> params{};

  while (true) {
    auto tok = m_lexer.Peek();
    CHECK_RET(tok);

    if (tok.value() == Token::rsquare_) {
      break;
    }

    auto type = ParseType();
    CHECK_RET(type);

    auto id = Lex();
    if (!id.status())
      return {ParseStatus::End};

    if (id.value() != Token::id_) {
      IssueDiagnostic(id.value(), "Unexpected token {}", id.value().ToString());
    }

    params.push_back(std::make_unique<VarDecl>(type.value().Tok(), type.value(),
                                               Id{id.value()}));

    if (tok.value() != Token::comma_) {
      IssueDiagnostic(tok.value(), "Unexpected token {}",
                      id.value().ToString());
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

    if (tok.value() == Token::end_)
      break;

    auto stmt = ParseStatement();
    CHECK_RET(stmt);
    body.push_back(std::move(stmt.value()));
  }

  auto func =
      std::make_unique<FuncDecl>(func_tok.value(), Id{id.value()}, ret.value(),
                                 std::move(params), std::move(body));
  return {ParseStatus::Continue, std::unique_ptr<ASTNode>{std::move(func)}};
}

AST Parser::Parse() {
  auto node = ParseTopLevelDecl();

  return {std::move(node.value())};
}
}
