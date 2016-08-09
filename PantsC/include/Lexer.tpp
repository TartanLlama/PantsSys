#pragma once

#include "DependentFalse.hpp"

namespace pants {
    template <char C>
    Lexer::Maybe<Token> Lexer::LexMultiToken() {
        static_assert(DependentFalse<std::integral_constant<char, C>>::value, "Not implemented");	
	}

	template<> 
	inline Lexer::Maybe<Token> Lexer::LexMultiToken<'-'>() {
	    (void)GetChar();
	    auto mc = PeekChar();
		if (mc.status() && mc.value() == '>') {
		    (void)GetChar();
			return MakeToken(Token::arrow_);
		}

		return MakeToken(Token::min_);
	}
	
	template<> 
	inline Lexer::Maybe<Token> Lexer::LexMultiToken<'i'>() {
	    (void)GetChar();
		auto mc = PeekChar();
		if (!mc.status()) {
		    return MakeToken(Token::id_, "i");
		}

		switch (mc.value()) {
		case 'n': return CheckedMakeToken(Token::in_, "in");
		case 's': return CheckedMakeToken(Token::is_, "is");
		case 'f': return CheckedMakeToken(Token::if_, "if");

		case '8': return CheckedMakeToken(Token::i8_, "i8");
		case '1': return LexStringToken(Token::i16_, "i16", "i");
		case '3': return LexStringToken(Token::i32_, "i32", "i");

		case 'm': return LexStringToken(Token::import_, "import", "i");

		default: return LexId("i");
		}
	}
	
	template<> 
	inline Lexer::Maybe<Token> Lexer::LexMultiToken<'e'>() {
	    (void)GetChar();
		auto mc = PeekChar();
		if (!mc.status()) {
		    return MakeToken(Token::id_, "e");
		}

		switch (mc.value()) {
		case 'l': return LexStringToken(Token::else_, "else", "e"); 
		case 'n': {
		    (void)GetChar();
			auto mc = PeekChar();
			if (!mc.status()) {
			    return MakeToken(Token::id_, "en");
			}

			switch (mc.value()) {
			case 'u': return LexStringToken(Token::enum_, "enum", "en");
			case 'd': return CheckedMakeToken(Token::end_, "end");
			}
		}

		default: return LexId("e");
		}
	}
	
	template<> 
	inline Lexer::Maybe<Token> Lexer::LexMultiToken<'f'>() {
	    (void)GetChar();
		auto mc = PeekChar();
		if (!mc.status()) {
		    return MakeToken(Token::id_, "f");
		}

		switch (mc.value()) {
		case 'o': return LexStringToken(Token::for_, "for", "f");
		case 'a': return LexStringToken(Token::false_, "false", "f");
		case 'u': return LexStringToken(Token::func_, "func", "f");

		default: return LexId("f");
		}
	}

	template<> 
	inline Lexer::Maybe<Token> Lexer::LexMultiToken<'c'>() {
	    (void)GetChar();
		auto mc = PeekChar();
		if (!mc.status()) {
		    return MakeToken(Token::id_, "c");
		}

		switch (mc.value()) {
		case 'h': return LexStringToken(Token::char_, "char", "c");
		case 'l': return LexStringToken(Token::class_, "class", "c");
		
		default: return LexId("c");
		}
	}
	
	template<> 
	inline Lexer::Maybe<Token> Lexer::LexMultiToken<'t'>() {
	    (void)GetChar();
		auto mc = PeekChar();
		if (!mc.status()) {
		    return MakeToken(Token::id_, "t");
		}

		switch (mc.value()) {
		case 'e': return LexStringToken(Token::template_, "template", "t");
		case 'r': return LexStringToken(Token::true_, "true", "t");

		default: return LexId("t");
		}
	}
	
	template<> 
	inline Lexer::Maybe<Token> Lexer::LexMultiToken<'='>() {
		(void)GetChar();
		auto mc = PeekChar();
		if (mc.status() && mc.value() == '=') {
		    (void)GetChar();
			return MakeToken(Token::eq_);
		}

		return MakeToken(Token::assign_);
	}
	
	template<> 
	inline Lexer::Maybe<Token> Lexer::LexMultiToken<'<'>() {
		(void)GetChar();
		auto mc = PeekChar();
		if (mc.status() && mc.value() == '=') {
		    (void)GetChar();
			return MakeToken(Token::le_);
		}

		return MakeToken(Token::lt_);
	}
	
	template<> 
	inline Lexer::Maybe<Token> Lexer::LexMultiToken<'>'>() {
		(void)GetChar();
		auto mc = PeekChar();
		if (mc.status() && mc.value() == '=') {
		    (void)GetChar();
			return MakeToken(Token::ge_);
		}

		return MakeToken(Token::gt_);
	}

	template<>
	inline Lexer::Maybe<Token> Lexer::LexMultiToken<'u'>() {
	    (void)GetChar();
		auto mc = PeekChar();

		if (!mc.status()) {
		    return MakeToken(Token::id_, "u");
		}

		switch (mc.value()) {
		    case '8': return CheckedMakeToken(Token::u8_, "u8");
		    case '1': return LexStringToken(Token::u16_, "u16", "u");
		    case '3': return LexStringToken(Token::u32_, "u32", "u");

			default: return LexId("u");
		}
	}
}