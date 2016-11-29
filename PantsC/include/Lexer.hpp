#pragma once
#include <iostream>
#include <queue>

#include "Tokens.hpp"
#include "status_value.hpp"
#include "Diagnostic.hpp"

namespace pants {
    class LexStatus {
    public:
        enum Status { End, Continue, Unrecognised };
        LexStatus(Status s) : m_status{ s } {}
        LexStatus(const LexStatus& rhs) : m_status{ Status{ rhs } } {}
        operator bool() const { return m_status != End; }
        operator Status() const { return m_status; }
        LexStatus& operator= (Status s) { m_status = s; return *this; }

    private:
        Status m_status;
    };

    class Lexer {
    public:
        template <typename T>
        using Maybe = status_value<LexStatus, T>;

        Lexer(std::istream& is)
            : m_is{ is }, m_row{ 0 }, m_col{ 0 }
        {}
        Maybe<Token> Lex();
        Maybe<Token> Peek();
        Maybe<Token> PeekMore();
        const std::vector<Diagnostic>& diags() const { return m_diags; }
    private:
        Maybe<Token> LexImpl();

        Maybe<char> GetChar();
        Maybe<char> PeekChar();
        void UngetChar() { m_is.unget(); }

        Maybe<Token> LexId(std::string so_far = "");
        Maybe<Token> LexHex();
        Maybe<Token> LexInt();
        Maybe<Token> LexCharToken(Token::Kind kind);
        Maybe<Token> LexStringToken(Token::Kind kind, const std::string& str, std::string so_far="");
        template <char C> Maybe<Token> LexMultiToken();

        void EatWhitespace();

        template <typename... Args>
        Maybe<Token> MakeToken(Token::Kind kind, Args&&... args) {
            return{ LexStatus::Continue,
                {kind, m_row, m_col, std::forward<Args>(args)... } };
        }

        Maybe<Token> CheckedMakeToken(Token::Kind kind, const std::string& str);

        std::istream& m_is;
        std::size_t m_row, m_col;

        template <typename... Ts>
        void IssueDiagnostic(const std::string& fmt_str, const Ts&... ts) {
            m_diags.emplace_back(m_row, m_col, fmt::format(fmt_str, ts...));
        }

        std::vector<Diagnostic> m_diags;
        std::queue<Maybe<Token>> m_peeked;
    };
}

#include "Lexer.tpp"
