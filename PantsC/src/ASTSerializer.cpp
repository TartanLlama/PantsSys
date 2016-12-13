#include "ASTSerializer.hpp"

namespace pants{
namespace {
template <typename T> struct IdFor;
template<> struct IdFor<Id> { static constexpr uint32_t value = 0; };
template<> struct IdFor<Int> { static constexpr uint32_t value = 1; };
template<> struct IdFor<Bool> { static constexpr uint32_t value = 2; };
template<> struct IdFor<VarDecl> { static constexpr uint32_t value = 3; };
template<> struct IdFor<FuncDecl> { static constexpr uint32_t value = 4; };
template<> struct IdFor<ClassDecl> { static constexpr uint32_t value = 5; };
template<> struct IdFor<EnumDecl> { static constexpr uint32_t value = 6; };
template<> struct IdFor<For> { static constexpr uint32_t value = 7; };
template<> struct IdFor<While> { static constexpr uint32_t value = 8; };
template<> struct IdFor<Expr> { static constexpr uint32_t value = 9; };
template<> struct IdFor<If> { static constexpr uint32_t value = 10; };
template<> struct IdFor<Return> { static constexpr uint32_t value = 11; };
template<> struct IdFor<BinaryOp> { static constexpr uint32_t value = 12; };
template<> struct IdFor<Call> { static constexpr uint32_t value = 13; };
template<> struct IdFor<UnaryOp> { static constexpr uint32_t value = 14; };
template<> struct IdFor<Type> { static constexpr uint32_t value = 15; };
template<> struct IdFor<AST> { static constexpr uint32_t value = 16; };
template<> struct IdFor<ASTNode> { static constexpr uint32_t value = 17; };

template <typename T> constexpr uint32_t id_for = IdFor<T>::value;
}

void ASTSerializer::Visit(ASTNode& node) {
    m_os << id_for<ASTNode> << '\0';
    auto tok = node.Tok();
    m_os << tok.Type() << '\0' << tok.Row() << '\0' << tok.Col() << '\0';
}

void ASTSerializer::Visit(Id &node) {
    m_os << id_for<Id> << '\0';
    Visit(static_cast<ASTNode&>(node));
    m_os << node.GetString() << '\0';
}

void ASTSerializer::Visit(Int &node) {
    m_os << id_for<Int> << '\0';
    Visit(static_cast<ASTNode&>(node));
    m_os << node.GetInt() << '\0';
}

void ASTSerializer::Visit(Bool &node) {
    m_os << id_for<Bool> << '\0';
    Visit(static_cast<ASTNode&>(node));
    m_os << (node.Tok() == Token::true_ ? uint8_t{1} : uint8_t{0}) << '\0';
}

void ASTSerializer::Visit(VarDecl &node) {
    m_os << id_for<VarDecl> << '\0';
    Visit(static_cast<ASTNode&>(node));
    node.GetType().Accept(*this);
    node.GetId().Accept(*this);

    m_os << static_cast<bool>(node.GetInit()) << '\0';
    if (node.GetInit()) node.GetInit()->Accept(*this);
}

void ASTSerializer::Visit(FuncDecl &node) {
    m_os << id_for<FuncDecl> << '\0';
    Visit(static_cast<ASTNode&>(node));
    node.GetName().Accept(*this);
    node.GetType().Accept(*this);

    m_os << node.GetParams().size() << '\0';
    for (auto&& param : node.GetParams()) {
        param->Accept(*this);
    }

    m_os << node.GetBody().size() << '\0';
    for (auto&& statement : node.GetBody()) {
        statement->Accept(*this);
    }
}

void ASTSerializer::Visit(ClassDecl &node) {
    m_os << id_for<ClassDecl> << '\0';
    Visit(static_cast<ASTNode&>(node));
    m_os << node.GetVars().size() << '\0';
    for (auto&& var : node.GetVars()) {
        var->Accept(*this);
    }
}

void ASTSerializer::Visit(EnumDecl &node) {
    m_os << id_for<EnumDecl> << '\0';
    Visit(static_cast<ASTNode&>(node));
    m_os << node.GetEnums().size() << '\0';
    for (auto&& en : node.GetEnums()) {
        en.Accept(*this);
    }
}

void ASTSerializer::Visit(For &node) {
    m_os << id_for<For> << '\0';
    Visit(static_cast<ASTNode&>(node));
    (void)node;
}
void ASTSerializer::Visit(While &node) {
    m_os << id_for<While> << '\0';
    Visit(static_cast<ASTNode&>(node));
    (void)node;
}
void ASTSerializer::Visit(Expr &node) {
    m_os << id_for<Expr> << '\0';
    Visit(static_cast<ASTNode&>(node));
    (void)node;
}

void ASTSerializer::Visit(If &node) {
    m_os << id_for<If> << '\0';
    Visit(static_cast<ASTNode&>(node));
    m_os << node.GetConds().size() << '\0';
    for (auto&& cond : node.GetConds()) {
        cond.first->Accept(*this);

        m_os << cond.second.size() << '\0';
        for (auto&& el : cond.second) {
            el->Accept(*this);
        }
    }

    m_os << node.GetElse().size() << '\0';
    for (auto&& statement : node.GetElse()) {
        statement->Accept(*this);
    }
}

void ASTSerializer::Visit(Return &node) {
    m_os << id_for<Return> << '\0';
    Visit(static_cast<ASTNode&>(node));
    node.GetValue().Accept(*this);
}

void ASTSerializer::Visit(BinaryOp &node) {
    m_os << id_for<BinaryOp> << '\0';
    Visit(static_cast<ASTNode&>(node));
    node.GetLhs().Accept(*this);
    node.GetRhs().Accept(*this);
}

void ASTSerializer::Visit(Call &node) {
    m_os << id_for<Call> << '\0';
    Visit(static_cast<ASTNode&>(node));
    node.GetCallee().Accept(*this);

    m_os << node.GetArgs().size() << '\0';
    for (auto&& arg : node.GetArgs()) {
        arg->Accept(*this);
    }
}

void ASTSerializer::Visit(UnaryOp &node) {
    m_os << id_for<UnaryOp> << '\0';
    Visit(static_cast<ASTNode&>(node));
    (void)node;
}

void ASTSerializer::Visit(Type &node) {
    m_os << id_for<Type> << '\0';
    Visit(static_cast<ASTNode&>(node));
    m_os << node.Tok().ToString() << '\0';
}
}
