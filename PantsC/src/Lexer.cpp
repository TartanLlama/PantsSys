#include "Lexer.hpp"
#include "Tokens.hpp"

#include <stdexcept>

namespace {
bool ishex(char c) {
    c = tolower(c);
    return isdigit(c) || c == 'a' || c == 'b' || c == 'c' || c == 'd' ||
           c == 'e' || c == 'f';
}
}

namespace pants {
char Lexer::GetChar() {
    char c = m_is.get();
    if (!m_is)
        return EOF;

    if (c == '\n') {
        ++m_row;
        m_col = 0;
    } else {
        ++m_col;
    }

    return c;
}

char Lexer::PeekChar() {
    char c = m_is.peek();
    if (!m_is)
        return EOF;

    return c;
}

void Lexer::EatWhitespace() {
    while (true) {
        auto mc = PeekChar();
        if (mc == EOF)
            return;

        if (!isspace(mc))
            return;
        (void)GetChar();
    }
}

Token Lexer::LexCharToken(Token::Kind kind) {
    (void)GetChar();
    return MakeToken(kind);
}

Token Lexer::LexId(std::string so_far) {
    while (true) {
        auto mc = PeekChar();
        if (mc == EOF) {
            return MakeToken(Token::id_, so_far);
        }

        if (!isalnum(mc)) {
            return MakeToken(Token::id_, so_far);
        }

        (void)GetChar();
        so_far += mc;
    }
}

Token Lexer::LexInt() {
    auto mc = GetChar();
    std::string str_i{mc};

    while (true) {
        auto mc = PeekChar();
        if (mc == EOF) {
            return MakeToken(Token::int_, std::stoi(str_i));
        }

        if (isdigit(mc)) {
            (void)GetChar();
            str_i += mc;
        } else if (isalpha(mc)) {
            IssueDiagnostic("Malformed int");
            return MakeToken(Token::int_, std::stoi(str_i));
        } else {
            break;
        }
    }

    return MakeToken(Token::int_, std::stoi(str_i));
}

Token Lexer::LexHex() {
    std::string str_i{GetChar()};

    auto mc = PeekChar();
    if (mc == EOF) {
        return MakeToken(Token::int_, 0);
    }

    if (isdigit(mc)) {
        IssueDiagnostic("Ints cannot begin with a 0");
        return MakeToken(Token::int_, 0);
    }

    if (mc != 'x' && mc != 'X') {
        return MakeToken(Token::int_, 0);
    }
    (void)GetChar();

    while (true) {
        auto mc = PeekChar();
        if (mc == EOF) {
            return MakeToken(Token::int_, std::stoi(str_i, 0, 16));
        }

        if (ishex(mc)) {
            (void)GetChar();
            str_i += mc;
        } else if (isalpha(mc)) {
            IssueDiagnostic("Malformed int");
            return MakeToken(Token::int_, std::stoi(str_i, 0, 16));
        } else if (isspace(mc)) {
            break;
        }
    }

    return MakeToken(Token::int_, std::stoi(str_i, 0, 16));
}

Token Lexer::LexStringToken(Token::Kind kind,
                                          const std::string &str,
                                          std::string so_far) {
    auto mc = GetChar();
    so_far += mc;

    auto size = str.size();
    for (std::size_t i = so_far.size(); i < size; ++i) {
        auto mc = PeekChar();
        if (mc == EOF) {
            return LexId(so_far);
        }

        if (mc != str[i]) {
            return LexId(so_far);
        }

        (void)GetChar();
        so_far += mc;
    }
    UngetChar();
    return CheckedMakeToken(kind, str);
}

Token Lexer::PeekMore() {
    auto tok = LexImpl();

#ifdef TRACE_LEXING
    fmt::print("Lexed {}\n", token.ToString());
#endif

    m_peeked.push(tok);

    return tok;
}

Token Lexer::Peek() {
    if (!m_peeked.empty()) {
        return m_peeked.front();
    }

    auto tok = LexImpl();

#ifdef TRACE_LEXING
    fmt::print("Lexed {}\n", token.ToString());
#endif

    m_peeked.push(tok);

    return tok;
}

Token Lexer::Lex() {
    if (!m_peeked.empty()) {
        auto peeked = m_peeked.front();
        m_peeked.pop();
        return peeked;
    }

    auto tok = LexImpl();

#ifdef TRACE_LEXING
    fmt::print("Lexed {}\n", token.ToString());
#endif

    return tok;
}

Token Lexer::LexImpl() {
    EatWhitespace();
    auto mc = PeekChar();
    if (mc == EOF) {
        return MakeToken(Token::eof_);
    }

    auto c = mc;
    switch (c) {
    case '+':
        return LexCharToken(Token::add_);
    case '-':
        return LexMultiToken<'-'>(); // - ->
    case '/':
        return LexCharToken(Token::div_);
    case '*':
        return LexCharToken(Token::mul_);

    case ',':
        return LexCharToken(Token::comma_);
    case ':':
        return LexCharToken(Token::colon_);
    case ';':
        return LexCharToken(Token::semi_);
    case '.':
        return LexCharToken(Token::dot_);

    case '(':
        return LexCharToken(Token::lparen_);
    case ')':
        return LexCharToken(Token::rparen_);
    case '[':
        return LexCharToken(Token::lsquare_);
    case ']':
        return LexCharToken(Token::rsquare_);

    case 'a':
        return LexStringToken(Token::and_, "and");
    case 'o':
        return LexStringToken(Token::or_, "or");
    case 'n':
        return LexMultiToken<'n'>();
    case 'w':
        return LexStringToken(Token::while_, "while");
    case 'r':
        return LexStringToken(Token::return_, "return");
    case 'i':
        return LexMultiToken<'i'>(); // import in is if i8 i16 i32
    case 'f':
        return LexMultiToken<'f'>(); // func for false
    case 'e':
        return LexMultiToken<'e'>(); // enum else end
    case 'c':
        return LexMultiToken<'c'>(); // class char
    case 't':
        return LexMultiToken<'t'>(); // template true
    case 'd':
        return LexStringToken(Token::do_, "do");
    case 'b':
        return LexStringToken(Token::bool_, "bool");
    case 'u':
        return LexMultiToken<'u'>(); // u8 u16 u32

    case '>':
        return LexMultiToken<'>'>(); // > >=
    case '<':
        return LexMultiToken<'<'>(); // < <=
    case '=':
        return LexMultiToken<'='>(); // = ==

    case '0':
        return LexHex();

    default:
        break;
    }

    if (isalpha(c)) {
        return LexId();
    }

    if (isdigit(c)) {
        return LexInt();
    }

    throw std::runtime_error("Unrecognised character");
}

Token Lexer::CheckedMakeToken(Token::Kind kind,
                                            const std::string &str) {
    (void)GetChar();
    auto mc = PeekChar();
    if (mc != EOF && isalnum(mc)) {
        return LexId(str);
    }
    return MakeToken(kind);
}
}
