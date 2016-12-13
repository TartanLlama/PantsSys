#pragma once

#include "AST.hpp"

#include <iostream>

namespace pants {
class ASTSerializer : public ASTVisitor {
public:
    ASTSerializer(std::ostream& os) : m_os{os} {}

    void Visit(Id &) override;
    void Visit(Int &) override;
    void Visit(Bool &) override;
    void Visit(VarDecl &) override;
    void Visit(FuncDecl &) override;
    void Visit(ClassDecl &) override;
    void Visit(EnumDecl &) override;
    void Visit(For &) override;
    void Visit(While &) override;
    void Visit(Expr &) override;
    void Visit(If &) override;
    void Visit(Return &) override;
    void Visit(BinaryOp &) override;
    void Visit(Call &) override;
    void Visit(UnaryOp &) override;
    void Visit(Type &) override;
    void Visit(ASTNode &);

    using ASTVisitor::Visit;

private:
    std::ostream& m_os;
};
}
