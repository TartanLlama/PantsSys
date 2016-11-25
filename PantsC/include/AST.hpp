#pragma once

#include <memory>
#include <vector>

#include "Type.hpp"
#include "Tokens.hpp"

namespace pants {
    enum class BinOpSym {
        Add, Sub, Mul, Div, Lt, Le, Gt, Ge, Eq
            };

    enum class UnaryOpSym {
        Min, Not
            };

    class ASTNode {
    public:
        explicit ASTNode(Token tok) : m_tok{tok} {}
    protected:
        Token m_tok;
    };

    using ASTNodeUP = std::unique_ptr<ASTNode>;

    class Id : public ASTNode {
    public:
        Id(Token tok) : ASTNode{tok} {}
        std::string String() { return m_tok.String().value(); }
    };

    class VarDecl : public ASTNode {
        Id m_type;
        Id m_id;
    };
    using VarDeclUP = std::unique_ptr<VarDecl>;

    class FuncDecl : public ASTNode
    {
    public:
        FuncDecl(Token tok, Id name, Id type, std::vector<VarDeclUP> params, std::vector<ASTNodeUP> body) :
            ASTNode{tok}, m_name{name}, m_type{type}, m_params{std::move(params)}, m_body{std::move(body)}
        {}

    private:
        Id m_name;
        Id m_type;
        std::vector<VarDeclUP> m_params;
        std::vector<ASTNodeUP> m_body;
    };

    class ClassDecl : public ASTNode {
        Id m_name;
        std::vector<VarDeclUP> m_vars;
    };

    class EnumDecl : public ASTNode {
        Id m_name;
        std::vector<Id> m_enums;
    };

    class For : public ASTNode
    {
        Id m_id;
        ASTNodeUP m_range;
        std::vector<ASTNodeUP> m_body;
    };

    class While : public ASTNode {
        ASTNodeUP m_cond;
        std::vector<ASTNodeUP> m_body;
    };

    class Expr : public ASTNode{};
    using ExprUP = std::unique_ptr<Expr>;

    class BinaryOp : public Expr
    {
        ASTNodeUP m_lhs;
        ASTNodeUP m_rhs;
        BinOpSym m_op;
    };

    class UnaryOp : public Expr
    {
        ASTNodeUP m_lhs;
        ASTNodeUP m_rhs;
        BinOpSym m_op;
    };


    class Assign : public ASTNode {
        ASTNodeUP lhs;
        ASTNodeUP rhs;
    };

    class If : public ASTNode
    {
    private:
        ASTNodeUP m_cond;
        std::vector<ASTNodeUP> m_if;
        std::vector<ASTNodeUP> m_else;
    };

    class Call : public Expr
    {
    private:
        FuncDecl& m_func;
        std::vector<ASTNodeUP> m_args;
    };

    class Return : public ASTNode
    {
    private:
        FuncDecl& m_func;
        std::vector<ExprUP> m_args;
    };


    class AST {
    public:
        AST(ASTNodeUP node) : m_node{ std::move(node) } {}
    private:
        ASTNodeUP m_node;
    };
}
