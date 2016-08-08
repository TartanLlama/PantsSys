#include "Tokens.hpp"
#include "Lexer.hpp"

namespace {
	bool ishex(char c) {
		c = tolower(c);
		return isdigit(c) || c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f';
	}
}

namespace pants {
	Lexer::Maybe<char> Lexer::GetChar() {
		char c = m_is.get();
		if (!m_is) return{ LexStatus::End };

		if (c == '\n') {
			++m_row;
			m_col = 0;
		}
		else {
			++m_col;
		}

		return{ LexStatus::Continue, c };
	}

	Lexer::Maybe<char> Lexer::PeekChar() {
		char c = m_is.peek();
		if (!m_is) return{ LexStatus::End };

		return{ LexStatus::Continue, c };
	}

	void Lexer::EatWhitespace() {
		while (true) {
			auto mc = PeekChar();
			if (!mc.status()) return;

			if (!isspace(mc.value())) return;
			(void)GetChar();
		}
	}

	Lexer::Maybe<Token> Lexer::LexCharToken(Token::Kind kind) {
		(void)GetChar();
		return MakeToken(kind);
	}

	Lexer::Maybe<Token> Lexer::LexId(std::string so_far) {
		while (true) {
			auto mc = PeekChar();
			if (!mc.status()) {
				return MakeToken(Token::id_, so_far);
			}

			if (!isalnum(mc.value())) {
				return MakeToken(Token::id_, so_far);
			}

			(void)GetChar();
			so_far += mc.value();
		}
	}

	Lexer::Maybe<Token> Lexer::LexInt() {
		auto mc = GetChar();
		std::string str_i{ mc.value() };

		while (true) {
			auto mc = PeekChar();
			if (!mc.status()) {
				return MakeToken(Token::int_, std::stoi(str_i));
			}

			if (isdigit(mc.value())) {
				(void)GetChar();
				str_i += mc.value();
			}
			else if (isalpha(mc.value())) {
				IssueDiagnostic("Malformed int");
				return MakeToken(Token::int_, std::stoi(str_i));
			}
			else if (isspace(mc.value())) {
				break;
			}
		}

		return MakeToken(Token::int_, std::stoi(str_i));
	}

	Lexer::Maybe<Token> Lexer::LexHex() {
		std::string str_i{ GetChar().value() };

		auto mc = PeekChar();
		if (!mc.status()) {
			return MakeToken(Token::int_, 0);
		}

		if (isdigit(mc.value())) {
			IssueDiagnostic("Ints cannot begin with a 0");
			return MakeToken(Token::int_, 0);
		}

		if (mc.value() != 'x' && mc.value() != 'X') {
			return MakeToken(Token::int_, 0);
		}
		(void)GetChar();

		while (true) {
			auto mc = PeekChar();
			if (!mc.status()) {
				return MakeToken(Token::int_, std::stoi(str_i, 0, 16));
			}

			if (ishex(mc.value())) {
				(void)GetChar();
				str_i += mc.value();
			}
			else if (isalpha(mc.value())) {
				IssueDiagnostic("Malformed int");
				return MakeToken(Token::int_, std::stoi(str_i, 0, 16));
			}
			else if (isspace(mc.value())) {
				break;
			}
		}

		return MakeToken(Token::int_, std::stoi(str_i, 0, 16));
	}

	Lexer::Maybe<Token> Lexer::LexStringToken(Token::Kind kind, const std::string& str, std::string so_far) {
		auto mc = GetChar();
		so_far += mc.value();

		auto size = str.size();
		for (std::size_t i = so_far.size(); i < size; ++i) {
			auto mc = PeekChar();
			if (!mc.status()) {
				return LexId(so_far);
			}

			if (mc.value() != str[i]) {
				return LexId(so_far);
			}

			(void)GetChar();
			so_far += mc.value();
		}
		UngetChar();
		return CheckedMakeToken(kind, str);
	}

	Lexer::Maybe<Token> Lexer::Lex() {
		EatWhitespace();
		auto mc = PeekChar();
		if (mc.status() == LexStatus::End) {
                      return{ LexStatus::End };
		}

		auto c = mc.value();
		switch (c) {
		case '+': return LexCharToken(Token::add_);
		case '-': return LexMultiToken<'-'>(); // - ->
		case '/': return LexCharToken(Token::div_);
		case '*': return LexCharToken(Token::mul_);

		case ',': return LexCharToken(Token::comma_);
		case ':': return LexCharToken(Token::colon_);
		case ';': return LexCharToken(Token::semi_);
		case '.': return LexCharToken(Token::dot_);

		case '(': return LexCharToken(Token::lparen_);
		case ')': return LexCharToken(Token::rparen_);
		case '[': return LexCharToken(Token::lsquare_);
		case ']': return LexCharToken(Token::rsquare_);
			
		case 'a': return LexStringToken(Token::and_, "and");
		case 'o': return LexStringToken(Token::or_, "or");
		case 'n': return LexStringToken(Token::not_, "not");
		case 'i': return LexMultiToken<'i'>(); // import in is if
		case 'f': return LexMultiToken<'f'>(); //"func, for, false"
		case 'e': return LexMultiToken<'e'>(); // "enum, else, end"
		case 'c': return LexStringToken(Token::class_, "class");
		case 't': return LexMultiToken<'t'>(); // "template, true"
		case 'd': return LexStringToken(Token::do_, "do");

		case '>': return LexMultiToken<'>'>(); // "> >="
		case '<': return LexMultiToken<'<'>(); // "< <="
		case '=': return LexMultiToken<'='>(); // "= =="

		case '0': return LexHex();

		default:
			break;
		}

		if (isalpha(c)) {
			return LexId();
		}

		if (isdigit(c)) {
			return LexInt();
		}

		return{ LexStatus::Unrecognised };
	}
    
	Lexer::Maybe<Token> Lexer::CheckedMakeToken(Token::Kind kind, const std::string& str) {
		(void)GetChar();
		auto mc = PeekChar();
		if (mc.status() && isalpha(mc.value())) {
			return LexId(str);
		}
		return MakeToken(kind);
	}
}
