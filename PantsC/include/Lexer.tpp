#pragma once

#include "DependentFalse.hpp"

namespace pants {
template <char C> Token Lexer::LexMultiToken() {
    static_assert(DependentFalse<std::integral_constant<char, C>>::value,
                  "Not implemented");
}

template <> inline Token Lexer::LexMultiToken<'-'>() {
    (void)GetChar();
    auto mc = PeekChar();
    if (mc != EOF && mc == '>') {
        (void)GetChar();
        return MakeToken(Token::arrow_);
    }

    return MakeToken(Token::min_);
}

template <> inline Token Lexer::LexMultiToken<'i'>() {
    (void)GetChar();
    auto mc = PeekChar();
    if (mc == EOF) {
        return MakeToken(Token::id_, "i");
    }

    switch (mc) {
    case 'n':
        return CheckedMakeToken(Token::in_, "in");
    case 's':
        return CheckedMakeToken(Token::is_, "is");
    case 'f':
        return CheckedMakeToken(Token::if_, "if");

    case '8':
        return CheckedMakeToken(Token::i8_, "i8");
    case '1':
        return LexStringToken(Token::i16_, "i16", "i");
    case '3':
        return LexStringToken(Token::i32_, "i32", "i");

    case 'm':
        return LexStringToken(Token::import_, "import", "i");

    default:
        return LexId("i");
    }
}

template <> inline Token Lexer::LexMultiToken<'e'>() {
    (void)GetChar();
    auto mc = PeekChar();
    if (mc == EOF) {
        return MakeToken(Token::id_, "e");
    }

    switch (mc) {
    case 'l':
        return LexStringToken(Token::else_, "else", "e");
    case 'n': {
        (void)GetChar();
        auto mc = PeekChar();
        if (mc == EOF) {
            return MakeToken(Token::id_, "en");
        }

        switch (mc) {
        case 'u':
            return LexStringToken(Token::enum_, "enum", "en");
        case 'd':
            return CheckedMakeToken(Token::end_, "end");
        }
    }

    default:
        return LexId("e");
    }
}

template <> inline Token Lexer::LexMultiToken<'f'>() {
    (void)GetChar();
    auto mc = PeekChar();
    if (mc == EOF) {
        return MakeToken(Token::id_, "f");
    }

    switch (mc) {
    case 'o':
        return LexStringToken(Token::for_, "for", "f");
    case 'a':
        return LexStringToken(Token::false_, "false", "f");
    case 'u':
        return LexStringToken(Token::func_, "func", "f");

    default:
        return LexId("f");
    }
}

template <> inline Token Lexer::LexMultiToken<'c'>() {
    (void)GetChar();
    auto mc = PeekChar();
    if (mc == EOF) {
        return MakeToken(Token::id_, "c");
    }

    switch (mc) {
    case 'h':
        return LexStringToken(Token::char_, "char", "c");
    case 'l':
        return LexStringToken(Token::class_, "class", "c");

    default:
        return LexId("c");
    }
}

template <> inline Token Lexer::LexMultiToken<'n'>() {
    (void)GetChar();
    auto mc = PeekChar();
    if (mc == EOF) {
        return MakeToken(Token::id_, "n");
    }

    switch (mc) {
    case 'o':
        return LexStringToken(Token::not_, "not", "n");
    case 'i':
        return LexStringToken(Token::nil_, "nil", "n");

    default:
        return LexId("c");
    }
}

template <> inline Token Lexer::LexMultiToken<'t'>() {
    (void)GetChar();
    auto mc = PeekChar();
    if (mc == EOF) {
        return MakeToken(Token::id_, "t");
    }

    switch (mc) {
    case 'e':
        return LexStringToken(Token::template_, "template", "t");
    case 'r':
        return LexStringToken(Token::true_, "true", "t");

    default:
        return LexId("t");
    }
}

template <> inline Token Lexer::LexMultiToken<'='>() {
    (void)GetChar();
    auto mc = PeekChar();
    if (mc != EOF && mc == '=') {
        (void)GetChar();
        return MakeToken(Token::eq_);
    }

    return MakeToken(Token::assign_);
}

template <> inline Token Lexer::LexMultiToken<'<'>() {
    (void)GetChar();
    auto mc = PeekChar();
    if (mc != EOF && mc == '=') {
        (void)GetChar();
        return MakeToken(Token::le_);
    }

    return MakeToken(Token::lt_);
}

template <> inline Token Lexer::LexMultiToken<'>'>() {
    (void)GetChar();
    auto mc = PeekChar();
    if (mc != EOF && mc == '=') {
        (void)GetChar();
        return MakeToken(Token::ge_);
    }

    return MakeToken(Token::gt_);
}

template <> inline Token Lexer::LexMultiToken<'u'>() {
    (void)GetChar();
    auto mc = PeekChar();

    if (mc == EOF) {
        return MakeToken(Token::id_, "u");
    }

    switch (mc) {
    case '8':
        return CheckedMakeToken(Token::u8_, "u8");
    case '1':
        return LexStringToken(Token::u16_, "u16", "u");
    case '3':
        return LexStringToken(Token::u32_, "u32", "u");

    default:
        return LexId("u");
    }
}
}
