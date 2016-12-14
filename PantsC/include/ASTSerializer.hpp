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
    void Visit(AST &) override;
    void Visit(Token &);

private:
    template <typename T>
    void Write(const T& t) {
        m_os.write(reinterpret_cast<const char*>(&t), sizeof(T));
    }
    std::ostream& m_os;
};

template<>
inline void ASTSerializer::Write(const std::string& s) {
    m_os << s << '\0';
}

}
