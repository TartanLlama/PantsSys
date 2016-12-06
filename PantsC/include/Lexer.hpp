#pragma once
#include <iostream>
#include <queue>

#include "Diagnostic.hpp"
#include "Tokens.hpp"
#include "status_value.hpp"

namespace pants {

/// A lexer for PantsLang

/// Tokenises a stream and exposes it as a series of [pants::Token]()s    
class Lexer {
  public:
    /// Constructs a lexer which reads from \param is
    Lexer(std::istream &is) : m_is{is}, m_row{0}, m_col{0} {}

    /// Read the next token from the stream used to initialise the lexer.
    ///
    /// \effects Advances the token stream
    Token Lex();

    /// Peek at the next token in the stream used to initialise the lexer.
    ///
    /// Subsequent calls to [pants::Lexer::Peek()]() will return the same value until [pants::Lexer::Lex()]()
    /// or [pants::Lexer::PeekMore()]() is called.
    ///
    Token Peek();

    /// Peek at the next token in the stream used to initialise the lexer.
    ///
    /// Subsequent calls to [pants::Lexer::PeekMore()]() will return new tokens.
    Token PeekMore();

    /// Get the diagnostics which have been emitted during lexing.
    const std::vector<Diagnostic> &diags() const { return m_diags; }

  private:
    Token LexImpl();

    /// Consumes the next character in the stream and returns it.
    char GetChar();

    /// Peeks at the next charecter in the stream without consuming it.
    char PeekChar();

    /// Puts the most recently read character back into the stream.
    void UngetChar() { m_is.unget(); }

    /// Lexes an identifier.
    /// An identifier is of form [a-zA-Z][a-zA-Z0-9]+
    /// \param so_far if part of the id has already been consumed, pass it here.
    Token LexId(std::string so_far = "");

    /// Lexes a hexidecimal literal of form 0x[0-9a-f]+
    Token LexHex();

    /// Lexes an integer literal of form [1-9][0-9]+
    Token LexInt();

    /// Does *not* check that the character is correct, client code should check this before calling.
    Token LexCharToken(Token::Kind kind);

//here
    
    /// Attempts to lex a token of the specified kind from the expected text.
    ///
    /// \param expected The string which is expected for the token
    /// E.g. "and" for pants::Token::and_.
    ///
    /// Assumes that the first character is correct, then checks all subsequent characters until it either matches
    /// the expected text, in which case it returns that token, or fails, in which case it returns an id token.
    Token LexStringToken(Token::Kind kind, const std::string &expected,
                                std::string so_far = "");

    /// Lex a token where \param C is the prefix of multiple possible tokens.
    ///
    /// Specializations must be added for each character which this is called for, otherwise a compile-time error
    /// will occur.
    template <char C> Token LexMultiToken();

    /// Discards all whitespace and comments from the current point in the stream until the next non-whitespace and
    /// non-comment character.
    void EatWhitespaceAndComments();

    /// Discards a comment from the current point in the stream.
    void EatComment();

    /// Discards a single-line comment (#) from the streame
    void EatSingleLineComment();

    /// Discards a multi-line comment (#{}#) from the stream.
    void EatMultiLineComment();

    /// Creates a token of the given kind and injects the current source code row and column.
    /// \param Args Any additional arguments needed to construct the token, e.g. value for pants::Token::Kind::int_ token
    template <typename... Args>
    Token MakeToken(Token::Kind kind, Args &&... args) {
        return {kind, m_row, m_col, std::forward<Args>(args)...};
    }

    /// Creates a token of the specified kind only if there are no more alphanumeric characters following it,
    /// otherwise an id token in returned.
    ///
    /// E.g. if "anda" is in the stream, `id(anda)` will be returned instead of `and, id(a)`.
    Token CheckedMakeToken(Token::Kind kind, const std::string &str);

    std::istream &m_is; //< The stream which will be lexed from.
    std::size_t m_row, m_col; //< The current row and column in the source text.

    /// Adds a diagnostic at the current lex position.
    template <typename... Ts>
    void IssueDiagnostic(const std::string &fmt_str, const Ts &... ts) {
        m_diags.emplace_back(m_row, m_col, fmt::format(fmt_str, ts...));
    }

    std::vector<Diagnostic> m_diags; //< Diagnostics which have been collected during lexing
    std::queue<Token> m_peeked; //< Stores the tokens which have been peeked at, but not lexed
};
}

#include "Lexer.tpp"
