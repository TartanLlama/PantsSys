#include "CodeGen.hpp"


namespace pants {
void CodeGen::EmitLabel (const std::string& label) {
    m_os << '.' << label << ":\n"
}

std::string CodeGen::GetMangledName (FuncDecl& func) {
    //TODO args
    return "_P_" + func.GetName();
}
    
void CodeGen::Visit(Token& tok) {
    Write(tok.Type());
    Write(tok.Row());
    Write(tok.Col());

    if (tok == Token::id_) Write(tok.String().value());
    if (tok == Token::int_) Write(tok.Int().value());
}

void CodeGen::Visit(Id &node) {
    Write(id_for<Id>);
    Visit(node.Tok());
}

void CodeGen::Visit(Int &node) {
    Write(id_for<Int>);
    Visit(node.Tok());
}

void CodeGen::Visit(Bool &node) {
    Write(id_for<Bool>);
    Visit(node.Tok());
}

void CodeGen::Visit(VarDecl &node) {
    Write(id_for<VarDecl>);
    Visit(node.Tok());
    node.GetType().Accept(*this);
    node.GetId().Accept(*this);

    Write(static_cast<bool>(node.GetInit()));
    if (node.GetInit()) node.GetInit()->Accept(*this);
}

void CodeGen::Visit(FuncDecl &node) {
    auto mangled_name = GetMangledName(node);
    EmitLabel(mangled_name);

    Write(node.GetParams().size());
    for (auto&& param : node.GetParams()) {
        param->Accept(*this);
    }

    Write(node.GetBody().size());
    for (auto&& statement : node.GetBody()) {
        statement->Accept(*this);
    }
}

void CodeGen::Visit(ClassDecl &node) {
    Write(id_for<ClassDecl>);
    Visit(node.Tok());
    Visit(node.GetName());
    Write(node.GetVars().size());
    for (auto&& var : node.GetVars()) {
        var->Accept(*this);
    }
}

void CodeGen::Visit(EnumDecl &node) {
    Write(id_for<EnumDecl>);
    Visit(node.Tok());
    Write(node.GetEnums().size());
    for (auto&& en : node.GetEnums()) {
        en.Accept(*this);
    }
}

void CodeGen::Visit(For &node) {
    //TODO
    Write(id_for<For>);
    Visit(node.Tok());
    (void)node;
}
void CodeGen::Visit(While &node) {
    //TODO
    Write(id_for<While>);
    Visit(node.Tok());
    (void)node;
}
void CodeGen::Visit(Expr &node) {
    //TODO
    Write(id_for<Expr>);
    Visit(node.Tok());
    (void)node;
}

void CodeGen::Visit(If &node) {
    Write(id_for<If>);
    Visit(node.Tok());
    Write(node.GetConds().size());
    for (auto&& cond : node.GetConds()) {
        cond.first->Accept(*this);

        Write(cond.second.size());
        for (auto&& el : cond.second) {
            el->Accept(*this);
        }
    }

    Write(node.GetElse().size());
    for (auto&& statement : node.GetElse()) {
        statement->Accept(*this);
    }
}

void CodeGen::Visit(Return &node) {
    Write(id_for<Return>);
    Visit(node.Tok());
    node.GetValue().Accept(*this);
}

void CodeGen::Visit(BinaryOp &node) {
    Write(id_for<BinaryOp>);
    Visit(node.Tok());
    node.GetLhs().Accept(*this);
    node.GetRhs().Accept(*this);
    Visit(node.GetOp());
}

void CodeGen::Visit(Call &node) {
    Write(id_for<Call>);
    Visit(node.Tok());
    node.GetCallee().Accept(*this);

    Write(node.GetArgs().size());
    for (auto&& arg : node.GetArgs()) {
        arg->Accept(*this);
    }
}

void CodeGen::Visit(UnaryOp &node) {
    //TODO
    Write(id_for<UnaryOp>);
    Visit(node.Tok());
    (void)node;
}

void CodeGen::Visit(Type &node) {
    Write(id_for<Type>);
    Visit(node.Tok());
}

void CodeGen::Visit(AST &ast) {
    Write(ast.GetNodes().size());
    for (auto&& node : ast.GetNodes()) {
        node->Accept(*this);
    }
}
}
