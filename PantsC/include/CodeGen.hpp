#include "AST.hpp"
#include "PantsIsa.hpp"
#include <iostream>

namespace pants {
enum class MemoryRegion {
    Register, Memory, Stack
};

class VarLocation {
public:
    using Address = uint32_t;

private:
    union {
        Register reg;
        Address addr;
    } m_location;
    MemoryRegion region;
};

class CodeGen : public ASTVisitor {
    public:
    CodeGen(std::ostream& os) : m_target{}, m_os{os} {}

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
    void Visit(AST &);


private:
    void EmitLabel (const std::string& label);

    template <typename T>
    T PrepareForEmit (const T& t) {
        return t;
    }

    std::string PrepareForEmit (Register reg) {
        return NameFromRegister(reg);
    }

    template <typename... Args>
    void EmitInst (const std::string& inst, Args&&... args) {
        m_os << inst;
        (void)std::initializer_list<int>{
            (m_os << ' ' << PrepareForEmit(args), 0)...
                };
        m_os << '\n';
    }


    std::string GetMangledName (FuncDecl& func);

    Register GetTarget() { return m_target; }
    void SetTarget(Register target) { m_target = target; }

    Register m_target;
    std::ostream& m_os;

};
}
