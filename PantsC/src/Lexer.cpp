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

void Lexer::EatMultiLineComment() {
    //assume that we've already read #{
    
    while (true) {
        auto c = PeekChar();

        if (c == EOF) return;

        if (c == '}') {
            (void)GetChar();
            if (PeekChar() == '#') {
                (void)GetChar();
                return;
            }
        }

        (void)GetChar();
    }
}

void Lexer::EatSingleLineComment() {
    //assume that we've already read #
    
    while (true) {
        auto c = PeekChar();

        if (c == EOF)
            return;

        if (c == '\n') {
            (void)GetChar();
            return;
        }

        (void)GetChar();
    }
}
    
void Lexer::EatComment() {
    if (PeekChar() == '#') (void)GetChar();

    if (PeekChar() == '{') {
        (void)GetChar();
        EatMultiLineComment();
    } 
    else {
        EatSingleLineComment();
    }
}

    
void Lexer::EatWhitespaceAndComments() {
    while (true) {
        auto c = PeekChar();
        if (c == EOF)
            return;

        if (c == '#') {
            EatComment();
        }
        else if (!isspace(c)) {
            return;
        } else {
            (void)GetChar();
        }
    }
}

Token Lexer::LexCharToken(Token::Kind kind) {
    (void)GetChar();
    return MakeToken(kind);
}

Token Lexer::LexId(std::string so_far) {
    while (true) {
        auto c = PeekChar();
        if (c == EOF) {
            return MakeToken(Token::id_, so_far);
        }

        if (!isalnum(c)) {
            return MakeToken(Token::id_, so_far);
        }

        (void)GetChar();
        so_far += c;
    }
}

Token Lexer::LexInt() {
    auto c = GetChar();
    std::string str_i{c};

    while (true) {
        auto ic = PeekChar();
        if (ic == EOF) {
            return MakeToken(Token::int_, std::stoi(str_i));
        }

        if (isdigit(ic)) {
            (void)GetChar();
            str_i += ic;
        } else if (isalpha(ic)) {
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

    auto c = PeekChar();
    if (c == EOF) {
        return MakeToken(Token::int_, 0);
    }

    if (isdigit(c)) {
        IssueDiagnostic("Ints cannot begin with a 0");
        return MakeToken(Token::int_, 0);
    }

    if (c != 'x' && c != 'X') {
        return MakeToken(Token::int_, 0);
    }
    (void)GetChar();

    while (true) {
        auto c = PeekChar();
        if (c == EOF) {
            return MakeToken(Token::int_, std::stoi(str_i, 0, 16));
        }

        if (ishex(c)) {
            (void)GetChar();
            str_i += c;
        } else if (isalpha(c)) {
            IssueDiagnostic("Malformed int");
            return MakeToken(Token::int_, std::stoi(str_i, 0, 16));
        } else if (isspace(c)) {
            break;
        }
    }

    return MakeToken(Token::int_, std::stoi(str_i, 0, 16));
}

Token Lexer::LexStringToken(Token::Kind kind,
                                          const std::string &str,
                                          std::string so_far) {
    auto c = GetChar();
    so_far += c;

    auto size = str.size();
    for (std::size_t i = so_far.size(); i < size; ++i) {
        auto ic = PeekChar();
        if (ic == EOF) {
            return LexId(so_far);
        }

        if (ic != str[i]) {
            return LexId(so_far);
        }

        (void)GetChar();
        so_far += ic;
    }
    UngetChar();
    return CheckedMakeToken(kind, str);
}

Token Lexer::PeekMore() {
    auto tok = LexImpl();

    m_peeked.push(tok);

    return tok;
}

Token Lexer::Peek() {
    if (!m_peeked.empty()) {
        return m_peeked.front();
    }

    auto tok = LexImpl();

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

    return tok;
}

Token Lexer::LexImpl() {
    EatWhitespaceAndComments();
    
    auto c = PeekChar();
    if (c == EOF) {
        return MakeToken(Token::eof_);
    }

    switch (c) {
    case '+':
        return LexCharToken(Token::add_);
    case '-':
        return LexMultiToken<'-'>(); // - ->
    case '/':
        return LexCharToken(Token::div_);
    case '*':
        return LexCharToken(Token::mul_);
    case '%':
        return LexCharToken(Token::mod_);
        
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
    auto c = PeekChar();
    if (c != EOF && isalnum(c)) {
        return LexId(str);
    }
    return MakeToken(kind);
}
}
