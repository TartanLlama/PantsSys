#pragma once

#include "AST.hpp"

namespace pants {
class ASTPrinter : public ASTVisitor {
    void Visit(Id &) override;
    void Visit(Int &) override;
    void Visit(VarDecl &) override;
    void Visit(FuncDecl &) override;
    void Visit(ClassDecl &) override;
    void Visit(EnumDecl &) override;
    void Visit(For &) override;
    void Visit(While &) override;
    void Visit(Expr &) override;
    void Visit(Assign &) override;
    void Visit(If &) override;
    void Visit(Return &) override;
    void Visit(BinaryOp &) override;
    void Visit(Call &) override;
    void Visit(UnaryOp &) override;
};
}
