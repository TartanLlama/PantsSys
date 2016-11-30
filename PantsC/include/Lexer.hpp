#pragma once
#include <iostream>
#include <queue>

#include "Diagnostic.hpp"
#include "Tokens.hpp"
#include "status_value.hpp"

namespace pants {
class Lexer {
  public:
    Lexer(std::istream &is) : m_is{is}, m_row{0}, m_col{0} {}
    Token Lex();
    Token Peek();
    Token PeekMore();
    const std::vector<Diagnostic> &diags() const { return m_diags; }

  private:
    Token LexImpl();

    char GetChar();
    char PeekChar();
    void UngetChar() { m_is.unget(); }

    Token LexId(std::string so_far = "");
    Token LexHex();
    Token LexInt();
    Token LexCharToken(Token::Kind kind);
    Token LexStringToken(Token::Kind kind, const std::string &str,
                                std::string so_far = "");
    template <char C> Token LexMultiToken();

    void EatWhitespace();

    template <typename... Args>
    Token MakeToken(Token::Kind kind, Args &&... args) {
        return {kind, m_row, m_col, std::forward<Args>(args)...};
    }

    Token CheckedMakeToken(Token::Kind kind, const std::string &str);

    std::istream &m_is;
    std::size_t m_row, m_col;

    template <typename... Ts>
    void IssueDiagnostic(const std::string &fmt_str, const Ts &... ts) {
        m_diags.emplace_back(m_row, m_col, fmt::format(fmt_str, ts...));
    }

    std::vector<Diagnostic> m_diags;
    std::queue<Token> m_peeked;
};
}

#include "Lexer.tpp"
