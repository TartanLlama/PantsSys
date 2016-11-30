#pragma once

#include <string>

namespace pants {
class Diagnostic {
  public:
    Diagnostic(std::size_t row, std::size_t col, std::string text)
        : m_row{row}, m_col{col}, m_text{text} {}

    Diagnostic(const Diagnostic &rhs) = default;
    Diagnostic &operator=(const Diagnostic &rhs) = default;

    std::size_t row() const { return m_row; }
    std::size_t col() const { return m_col; }
    const std::string &text() const { return m_text; }

  private:
    std::size_t m_row, m_col;
    std::string m_text;
};
}
