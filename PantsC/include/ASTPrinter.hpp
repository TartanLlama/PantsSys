#pragma once

#include "AST.hpp"

namespace pants {
class ASTPrinter : public ASTVisitor {
public:
    ASTPrinter(std::ostream& os) : m_os{os} {}
    
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
    void Visit(Assign &) override;
    void Visit(If &) override;
    void Visit(Return &) override;
    void Visit(BinaryOp &) override;
    void Visit(Call &) override;
    void Visit(UnaryOp &) override;
    void Visit(Type &) override;

    using ASTVisitor::Visit;

private:
    void up() { --m_depth; }
    void down();
    void print(const std::string& str);

    std::ostream& m_os;
    std::size_t m_depth = 0;
};
}
