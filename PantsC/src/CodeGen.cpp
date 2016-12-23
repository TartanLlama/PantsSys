#include "CodeGen.hpp"
#include "PantsIsa.hpp"


namespace pants {
void CodeGen::EmitLabel (const std::string& label) {
    m_os << '.' << label << ":\n";
}


std::string CodeGen::GetMangledName (FuncDecl& func) {
    //TODO args
    return std::string{"_P_"} + func.GetName().GetString();
}

void CodeGen::Visit(Id &node) {
    (void)node;
}

void CodeGen::Visit(Int &node) {
    auto target = GetTarget();
    EmitInst("load", target, node.GetInt());
}

void CodeGen::Visit(Bool &node) {
    auto target = GetTarget();
    auto val = node.Tok() == Token::true_;
    EmitInst("load", target, val);
}

void CodeGen::Visit(VarDecl &node) {
    //TODO
    (void)node;
}

void CodeGen::Visit(FuncDecl &node) {
    auto mangled_name = GetMangledName(node);
    EmitLabel(mangled_name);

    for (auto&& param : node.GetParams()) {
        //TODO
        (void)param;
    }

    for (auto&& statement : node.GetBody()) {
        statement->Accept(*this);
    }
}

void CodeGen::Visit(ClassDecl &node) {
    //TODO
    (void)node;
}

void CodeGen::Visit(EnumDecl &node) {
    //TODO
    (void)node;
}

void CodeGen::Visit(For &node) {
    //TODO
    (void)node;
}
void CodeGen::Visit(While &node) {
    //TODO
    (void)node;
}
void CodeGen::Visit(Expr &node) {
    //TODO
    (void)node;
}

void CodeGen::Visit(If &node) {
    //TODO
    (void)node;
}

void CodeGen::Visit(Return &node) {
    //TODO
    (void)node;
}

void CodeGen::Visit(BinaryOp &node) {
    auto target = GetTarget();

    if (node.GetOp() == Token::assign_) {
        SetTarget(Register::r1);
        node.GetRhs().Accept(*this);

        if (auto op = dynamic_cast<UnaryOp*>(&node.GetLhs())) {
            if (op->Tok() == Token::mul_) {
                auto& arg = op->GetArg();
                SetTarget(Register::r0);
                arg.Accept(*this);
                EmitInst("set", Register::me, Register::r0);
                EmitInst("store", Register::r1);
                return;
            }
            else {
                return;
                //TODO
            }
        }
        else {
            return;
            //TODO
        }
    }


    SetTarget(Register::r0);
    node.GetLhs().Accept(*this);

    SetTarget(Register::r1);
    node.GetRhs().Accept(*this);

    std::string inst = "";
    switch (node.GetOp().Type()) {
    case Token::add_: inst = "add"; break;
    case Token::min_: inst = "min"; break;
    case Token::div_: inst = "div"; break;
    case Token::mod_: inst = "mod"; break;
    case Token::mul_: inst = "mul"; break;
    case Token::lt_: inst = "lt"; break;
    case Token::le_: inst = "le"; break;
    case Token::gt_: inst = "gt"; break;
    case Token::ge_: inst = "ge"; break;
    case Token::eq_: inst = "eq"; break;
    case Token::and_: inst = "and"; break;
    case Token::or_: inst = "or"; break;
    default: throw std::runtime_error("Unhandled codegen operator");
    }

    EmitInst(inst, target, Register::r0, Register::r1);
}

void CodeGen::Visit(Call &node) {
    //TODO
    (void)node;
}

void CodeGen::Visit(UnaryOp &node) {
    //TODO
    (void)node;
}

void CodeGen::Visit(Type &node) {
    //TODO
    (void)node;
}

void CodeGen::Visit(AST &ast) {
    for (auto&& node : ast.GetNodes()) {
        node->Accept(*this);
    }
}
}
