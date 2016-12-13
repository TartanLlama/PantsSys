#include "ASTPrinter.hpp"
#include "fmt/format.h"
#include <iostream>

using fmt::format;

namespace pants {
void ASTPrinter::Visit(Id &node) {
    print(format("<<Id {}>>", node.GetString()));
}

void ASTPrinter::Visit(Int &node) {
    print(format("<<Int {}>>", node.GetInt()));
}

void ASTPrinter::Visit(Bool &node) {
    auto text = (node.Tok() == Token::true_ ? "true" : "false");
    print(format("<<Bool {}>>", text));
}

void ASTPrinter::Visit(VarDecl &node) {
    print("<<VarDecl>>");

    down();

    node.GetType().Accept(*this);
    node.GetId().Accept(*this);

    if (node.GetInit()) node.GetInit()->Accept(*this);

    up();
}
void ASTPrinter::Visit(FuncDecl &node) {
    print("<<FuncDecl>>");

    down();

    node.GetName().Accept(*this);
    node.GetType().Accept(*this);

    for (auto&& param : node.GetParams()) {
        param->Accept(*this);
    }

    for (auto&& statement : node.GetBody()) {
        statement->Accept(*this);
    }

    up();
}
void ASTPrinter::Visit(ClassDecl &node) {
    print(format("<<ClassDecl {}>>", node.GetName().GetString()));

    down();

    for (auto&& var : node.GetVars()) {
        var->Accept(*this);
    }

    up();
}
void ASTPrinter::Visit(EnumDecl &node) {
    print(format("<<EnumDecl {}>>", node.GetName().GetString()));

    down();

    for (auto&& en : node.GetEnums()) {
        en.Accept(*this);
    }

    up();
}

void ASTPrinter::Visit(For &node) {
    print("<<For>>");
    (void)node;
}
void ASTPrinter::Visit(While &node) {
    print("<<While>>");
    (void)node;
}
void ASTPrinter::Visit(Expr &node) {
    print("<<Expr>>");
    (void)node;
}

void ASTPrinter::Visit(If &node) {
    print("<<If>>");

    down();

    for (auto&& cond : node.GetConds()) {
        cond.first->Accept(*this);
        down();

        for (auto&& el : cond.second) {
            el->Accept(*this);
        }

        up();
    }


    for (auto&& statement : node.GetElse()) {
        statement->Accept(*this);
    }

    up();
}
void ASTPrinter::Visit(Return &node) {
    print("<<Return>>");
    down();
    node.GetValue().Accept(*this);
    up();
}
void ASTPrinter::Visit(BinaryOp &node) {
    print(format("<<BinaryOp {}>>", node.GetOp().ToString()));
    down();
    node.GetLhs().Accept(*this);
    node.GetRhs().Accept(*this);
    up();
}
void ASTPrinter::Visit(Call &node) {
    print("<<Call>>");
    down();
    node.GetCallee().Accept(*this);

    for (auto&& arg : node.GetArgs()) {
        arg->Accept(*this);
    }

    up();
}
void ASTPrinter::Visit(UnaryOp &node) {
    print("<<UnaryOp>>");
    (void)node;
}

void ASTPrinter::Visit(Type &node) {
    print(format("<<Type {}>>", node.Tok().ToString()));
}

void ASTPrinter::down() {
    ++m_depth;
}

void ASTPrinter::print(const std::string& str) {
    for (std::size_t i = 0; i < m_depth; ++i) {
        m_os << "  ";
    }

    m_os << str << '\n';
}

}
