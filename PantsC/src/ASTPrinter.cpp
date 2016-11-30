#include "ASTPrinter.hpp"
#include <iostream>

namespace pants {
void ASTPrinter::Visit(Id &node) {
    std::cout << "(Id " << node.String() << ')';
}
void ASTPrinter::Visit(Int &node) { std::cout << "(Int " << node.Val() << ')'; }
void ASTPrinter::Visit(VarDecl &node) {
    std::cout << "(VarDecl";
    std::cout << ")";
}
void ASTPrinter::Visit(FuncDecl &node) {
    std::cout << "(FuncDecl";
    node.Name().Accept(*this);
    node.Type().Accept(*this);

    for (auto it = node.ParamsBegin(), end = node.ParamsEnd(); it != end;
         ++it) {
        (*it)->Accept(*this);
    }

    for (auto it = node.BodyBegin(), end = node.BodyEnd(); it != end; ++it) {
        (*it)->Accept(*this);
    }

    std::cout << ")";
}
void ASTPrinter::Visit(ClassDecl &node) {
    std::cout << "(ClassDecl";
    std::cout << ")";
}
void ASTPrinter::Visit(EnumDecl &node) {
    std::cout << "(EnumDecl";
    std::cout << ")";
}
void ASTPrinter::Visit(For &node) {
    std::cout << "(For";
    std::cout << ")";
}
void ASTPrinter::Visit(While &node) {
    std::cout << "(While";
    std::cout << ")";
}
void ASTPrinter::Visit(Expr &node) {
    std::cout << "(Expr";
    std::cout << ")";
}
void ASTPrinter::Visit(Assign &node) {
    std::cout << "(Assign";
    std::cout << ")";
}
void ASTPrinter::Visit(If &node) {
    std::cout << "(If";
    std::cout << ")";
}
void ASTPrinter::Visit(Return &node) {
    std::cout << "(Return";
    std::cout << ")";
}
void ASTPrinter::Visit(BinaryOp &node) {
    std::cout << "(BinaryOp ";
    std::cout << node.Tok().ToString() << ' ';
    node.Lhs().Accept(*this);
    node.Rhs().Accept(*this);
    std::cout << ")";
}
void ASTPrinter::Visit(Call &node) {
    std::cout << "(Call";
    std::cout << ")";
}
void ASTPrinter::Visit(UnaryOp &node) {
    std::cout << "(UnaryOp";
    std::cout << ")";
}
}
