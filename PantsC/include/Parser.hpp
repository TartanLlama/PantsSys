#pragma once

#include "AST.hpp"

#include "fmt/format.h"
#include "status_value.hpp"
#include "Diagnostic.hpp"

namespace pants {
	class ParseStatus {
	public:
		enum Status { End, Continue };
		ParseStatus(Status s) : m_status{ s } {}
		ParseStatus(const ParseStatus& rhs) : m_status{ Status{rhs} } {}
		operator bool() const { return m_status != End; }
		operator Status() const { return m_status; }
		ParseStatus& operator= (Status s) { m_status = s; return *this; }

	private:
		Status m_status;
	};

	template <typename T>
	using Maybe = status_value<ParseStatus, T>;

	class Parser {
	public:
		Parser(std::istream& is);
		AST parse();

		Maybe<char> getChar();
		Maybe<bool> expectChar(char c);

		const std::vector<Diagnostic>& diags() const { return m_diags; }

		template <typename... Ts>
		void issueDiagnostic(const std::string& fmt_str, const Ts&... ts) {
			m_diags.emplace_back(m_row, m_col, fmt::format(fmt_str, ts...));
		}
	private:
		Maybe<ASTNodeUP> parseNode();

		std::istream& m_is;
		std::size_t m_row, m_col;
		std::vector<Diagnostic> m_diags;
	};
}