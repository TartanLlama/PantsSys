#include "AST.hpp"
#include "PantsIsa.hpp"

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
    CodeGen(std::ostream& os) : m_os{os} {}

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

    using ASTVisitor::Visit;

private:
    void EmitLabel (const std::string& label);
    std::string GetMangledName (FuncDecl& func);
    
    std::ostream& m_os;
    
};
}
