#pragma once
#include <cassert>
#include <stdexcept>
#include <string>

#include "fmt/format.h"
#include "optional.hpp"

namespace pants {
class Token {
  public:
    enum Kind {
        import_,
        func_,
        enum_,
        class_,
        template_,
        in_,
        is_,
        for_,
        while_,
        if_,
        else_,
        do_,
        end_,
        add_,
        min_,
        div_,
        mod_,
        mul_,
        lt_,
        le_,
        gt_,
        ge_,
        eq_,
        assign_,
        return_,
        and_,
        or_,
        not_,
        lparen_,
        rparen_,
        lsquare_,
        rsquare_,
        comma_,
        arrow_,
        semi_,
        dot_,
        colon_,
        ptr_,
        id_,
        int_,
        true_,
        false_,
        u8_,
        u16_,
        u32_,
        i8_,
        i16_,
        i32_,
        bool_,
        char_,
        nil_,
        eof_
    };

    Token(const Token &rhs)
        : m_kind{rhs.m_kind}, m_row{rhs.m_row}, m_col{rhs.m_col}, m_dummy{} {
        switch (m_kind) {
        case id_:
            new (&m_string) std::string{rhs.m_string};
            break;
        case int_:
            m_int = rhs.m_int;
            break;
        default:
            break;
        }
    }

    Token &operator=(const Token &rhs) {
        m_kind = rhs.m_kind;
        m_row = rhs.m_row;
        m_col = rhs.m_col;
        switch (m_kind) {
        case id_:
            new (&m_string) std::string{rhs.m_string};
            break;
        case int_:
            m_int = rhs.m_int;
            break;
        default:
            break;
        }

        return *this;
    }

    ~Token() {
        if (m_kind == id_)
            m_string.~basic_string();
    }
    Token(Kind kind, std::size_t row, std::size_t col)
        : m_kind{kind}, m_row{row}, m_col{col}, m_dummy{} {}

    Token(Kind kind, std::size_t row, std::size_t col, int data)
        : m_kind{kind}, m_row{row}, m_col{col}, m_int{data} {}
    Token(Kind kind, std::size_t row, std::size_t col, const std::string &data)
        : m_kind{kind}, m_row{row}, m_col{col}, m_string{data} {}

    util::optional<std::string> String() {
        if (m_kind != id_) {
            return {};
        }
        return m_string;
    }

    util::optional<int> Int() {
        if (m_kind != int_) {
            return {};
        }
        return m_int;
    }

    operator Kind() { return m_kind; }
    Kind Type() { return m_kind; }
    std::size_t Row() { return m_row; }
    std::size_t Col() { return m_col; }

    std::string ToString() {
        switch (m_kind) {
        case import_:
            return "import";
        case func_:
            return "func";
        case enum_:
            return "enum";
        case class_:
            return "class";
        case template_:
            return "template";
        case in_:
            return "in";
        case is_:
            return "is";
        case for_:
            return "for";
        case while_:
            return "while";
        case if_:
            return "if";
        case else_:
            return "else";
        case do_:
            return "do";
        case end_:
            return "end";
        case add_:
            return "add";
        case min_:
            return "min";
        case div_:
            return "div";
        case mul_:
            return "mul";
        case mod_:
            return "mod";
        case lt_:
            return "lt";
        case le_:
            return "le";
        case gt_:
            return "gt";
        case ge_:
            return "ge";
        case eq_:
            return "eq";
        case assign_:
            return "assign";
        case return_:
            return "return";
        case and_:
            return "and";
        case or_:
            return "or";
        case not_:
            return "not";
        case lparen_:
            return "lparen";
        case rparen_:
            return "rparen";
        case lsquare_:
            return "lsquare";
        case rsquare_:
            return "rsquare";
        case comma_:
            return "comma";
        case ptr_:
            return "ptr";
        case arrow_:
            return "arrow";
        case semi_:
            return "semi";
        case dot_:
            return "dot";
        case colon_:
            return "colon";
        case id_:
            return fmt::format("id({})", m_string);
        case int_:
            return fmt::format("int({})", m_int);
        case true_:
            return "true";
        case false_:
            return "false";
        case u8_:
            return "u8";
        case u16_:
            return "u16";
        case u32_:
            return "u32";
        case i8_:
            return "i8";
        case i16_:
            return "i16";
        case i32_:
            return "i32";
        case char_:
            return "char";
        case bool_:
            return "bool";
        case nil_:
            return "nil";
        case eof_:
            return "eof";
        default:
            throw std::runtime_error(fmt::format("Unhandled token kind {}", m_kind)); 
        }
    }

  private:
    Kind m_kind;
    std::size_t m_row, m_col;
    struct Dummy {};
    union {
        Dummy m_dummy;
        int m_int;
        std::string m_string;
    };

};
}
