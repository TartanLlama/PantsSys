#include "ASTPrinter.hpp"
#include "fmt/format.h"
#include <iostream>

using fmt::format;

namespace pants {
void ASTPrinter::Visit(Id &node) {
    print(format("<<Id {}>>", node.String()));
}

void ASTPrinter::Visit(Int &node) {
    print(format("<<Int {}>>", node.Val()));
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

    node.Name().Accept(*this);
    node.GetType().Accept(*this);

    for (auto it = node.ParamsBegin(), end = node.ParamsEnd(); it != end;
         ++it) {
        (*it)->Accept(*this);
    }

    for (auto it = node.BodyBegin(), end = node.BodyEnd(); it != end; ++it) {
        (*it)->Accept(*this);
    }

    up();
}
void ASTPrinter::Visit(ClassDecl &node) {
    print(format("<<ClassDecl {}>>", node.Name().String()));

    down();

    for (auto it = node.VarsBegin(), end = node.VarsEnd(); it != end; ++it) {
        (*it)->Accept(*this);
    }

    up();
}
void ASTPrinter::Visit(EnumDecl &node) {
    print(format("<<EnumDecl {}>>", node.Name().String()));

    down();

    for (auto it = node.EnumsBegin(), end = node.EnumsEnd(); it != end; ++it) {
        it->Accept(*this);
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
void ASTPrinter::Visit(Assign &node) {
    print("<<Assign>>");
    (void)node;
}
void ASTPrinter::Visit(If &node) {
    print("<<If>>");

    down();

    for (auto it = node.CondsBegin(), end = node.CondsEnd(); it != end; ++it) {
        it->first->Accept(*this);
        down();

        for (auto&& el : it->second) {
            el->Accept(*this);
        }

        up();
    }


    for (auto it = node.ElseBegin(), end = node.ElseEnd(); it != end; ++it) {
        (*it)->Accept(*this);
    }

    up();
}
void ASTPrinter::Visit(Return &node) {
    print("<<Return>>");
    down();
    node.Value().Accept(*this);
    up();
}
void ASTPrinter::Visit(BinaryOp &node) {
    print(format("<<BinaryOp {}>>", node.Tok().ToString()));
    down();
    node.Lhs().Accept(*this);
    node.Rhs().Accept(*this);
    up();
}
void ASTPrinter::Visit(Call &node) {
    print("<<Call>>");
    down();
    node.Callee().Accept(*this);

    for (auto it = node.ArgsBegin(), end = node.ArgsEnd(); it != end;
         ++it) {
        (*it)->Accept(*this);
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
        std::cout << "  ";
    }

    std::cout << str << '\n';
}

}
