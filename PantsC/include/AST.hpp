#pragma once

#include <memory>
#include <vector>

#include "Tokens.hpp"

namespace pants {
class Id;
class Int;
class Bool;
class VarDecl;
class FuncDecl;
class ClassDecl;
class EnumDecl;
class For;
class While;
class Expr;
class If;
class Return;
class BinaryOp;
class Call;
class UnaryOp;
class Type;
class AST;

class ASTVisitor {
  public:
    virtual ~ASTVisitor() = default;

    virtual void Visit(Id &) = 0;
    virtual void Visit(Int &) = 0;
    virtual void Visit(Bool &) = 0;
    virtual void Visit(VarDecl &) = 0;
    virtual void Visit(FuncDecl &) = 0;
    virtual void Visit(ClassDecl &) = 0;
    virtual void Visit(EnumDecl &) = 0;
    virtual void Visit(For &) = 0;
    virtual void Visit(While &) = 0;
    virtual void Visit(Expr &) = 0;
    virtual void Visit(If &) = 0;
    virtual void Visit(Return &) = 0;
    virtual void Visit(BinaryOp &) = 0;
    virtual void Visit(Call &) = 0;
    virtual void Visit(UnaryOp &) = 0;
    virtual void Visit(Type &) = 0;

    virtual void Visit(AST &);
};

class ASTNode {
  public:
    virtual void Accept(ASTVisitor &visitor) = 0;
    explicit ASTNode(Token tok) : m_tok{tok} {}
    Token& Tok() { return m_tok; }
    Token Tok() const { return m_tok; }

  protected:
    Token m_tok;
};

using ASTNodeUP = std::unique_ptr<ASTNode>;

class Expr : public ASTNode {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    Expr(Token tok) : ASTNode{tok} {}
};
using ExprUP = std::unique_ptr<Expr>;

class Id : public Expr {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    Id(Token tok) : Expr{tok} {}
    std::string GetString() { return m_tok.String().value(); }
};
using IdUP = std::unique_ptr<Id>;

class Int : public Expr {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    Int(Token tok) : Expr{tok} {}
    int GetInt() { return m_tok.Int().value(); }
};
using IntUP = std::unique_ptr<Int>;

class Bool : public Expr {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    Bool(Token tok) : Expr{tok} {}
};
using BoolUP = std::unique_ptr<Bool>;

class BinaryOp : public Expr {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    BinaryOp(Token tok, ASTNodeUP lhs, ASTNodeUP rhs, Token op)
        : Expr{tok}, m_lhs{std::move(lhs)}, m_rhs{std::move(rhs)}, m_op{op} {}

    ASTNode &GetLhs() { return *m_lhs; }
    ASTNode &GetRhs() { return *m_rhs; }
    Token &GetOp() { return m_op; }

  private:
    ASTNodeUP m_lhs;
    ASTNodeUP m_rhs;
    Token m_op;
};
using BinaryOpUP = std::unique_ptr<BinaryOp>;

class UnaryOp : public Expr {
public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    UnaryOp(Token tok, ASTNodeUP arg, Token op)
        : Expr{tok}, m_arg{std::move(arg)}, m_op{op} {}

    ASTNode &GetArg() { return *m_arg; }
    Token &GetOp() { return m_op; }

private:
    ASTNodeUP m_arg;
    Token m_op;
};
using UnaryOpUP = std::unique_ptr<UnaryOp>;

class Type : public ASTNode {
public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    Type(Token tok) : ASTNode{tok} {}
};
using TypeUP = std::unique_ptr<Type>;

class VarDecl : public ASTNode {
public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    VarDecl(Token tok, Type type, Id id, ExprUP init)
        : ASTNode{tok}, m_type{type}, m_id{id}, m_init{std::move(init)}
    {}
    VarDecl(Token tok, Type type, Id id)
        : ASTNode{tok}, m_type{type}, m_id{id}, m_init{nullptr}
    {}


    Type GetType() { return m_type; }
    Id GetId() { return m_id; }
    ASTNodeUP& GetInit() { return m_init; }

    private:
    Type m_type;
    Id m_id;
    ASTNodeUP m_init;
};
using VarDeclUP = std::unique_ptr<VarDecl>;

class FuncDecl : public ASTNode {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    FuncDecl(Token tok, Id name, Type type, std::vector<VarDeclUP> params,
             std::vector<ASTNodeUP> body)
        : ASTNode{tok}, m_name{name}, m_type{type}, m_params{std::move(params)},
          m_body{std::move(body)} {}

    Id& GetName() { return m_name; }
    Type& GetType() { return m_type; }
    auto& GetParams() { return m_params; }
    auto& GetBody() { return m_body; }

  private:
    Id m_name;
    Type m_type;
    std::vector<VarDeclUP> m_params;
    std::vector<ASTNodeUP> m_body;
};
using FuncDeclUP = std::unique_ptr<FuncDecl>;

class ClassDecl : public ASTNode {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    ClassDecl(Token tok, Id name, std::vector<VarDeclUP> vars)
        : ASTNode{tok}, m_name{name}, m_vars{std::move(vars)}
    {}

    Id& GetName() { return m_name; }
    auto& GetVars() { return m_vars; }

  private:
    Id m_name;
    std::vector<VarDeclUP> m_vars;
};
using ClassDeclUP = std::unique_ptr<ClassDecl>;

class EnumDecl : public ASTNode {
public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    EnumDecl (Token tok, Id name, std::vector<Id> enums) :
        ASTNode{tok}, m_name{name}, m_enums{std::move(enums)}
    {}

    Id& GetName() { return m_name; }
    auto& GetEnums() { return m_enums; }


private:
    Id m_name;
    std::vector<Id> m_enums;
};
using EnumDeclUP = std::unique_ptr<EnumDecl>;

class For : public ASTNode {
public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    For (Token tok, Id var_name, ASTNodeUP range, std::vector<ASTNodeUP> body) :
        ASTNode{tok}, m_var_name{var_name}, m_range{std::move(range)}, m_body{std::move(body)}
    {}


    Id GetVarName() { return m_var_name; }
    auto& GetRange() { return m_range; }
    auto& GetBody() { return m_body; }

private:
    Id m_var_name;
    ASTNodeUP m_range;
    std::vector<ASTNodeUP> m_body;
};
using ForUP = std::unique_ptr<For>;

class While : public ASTNode {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    While (Token tok, ExprUP cond, std::vector<ASTNodeUP> body)
        : ASTNode(tok), m_cond(std::move(cond)), m_body(std::move(body)) {}

    Expr& GetCond() { return *m_cond; }
    auto& GetBody() { return m_body; }

private:
    ExprUP m_cond;
    std::vector<ASTNodeUP> m_body;
};
using WhileUP = std::unique_ptr<While>;

class If : public ASTNode {
  public:
    using CondBodyPair = std::pair<ExprUP, std::vector<ASTNodeUP>>;

    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    If (Token tok, std::vector<CondBodyPair> conds, std::vector<ASTNodeUP> else_body) :
        ASTNode{tok}, m_conds{std::move(conds)}, m_else{std::move(else_body)}
    {}

    auto& GetConds() { return m_conds; }
    auto& GetElse() { return m_else; }

  private:
    std::vector<CondBodyPair> m_conds;
    std::vector<ASTNodeUP> m_else;
};
using IfUP = std::unique_ptr<If>;

class Call : public Expr {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    Call (Token tok, ExprUP callee, std::vector<ExprUP> args)
        : Expr(tok), m_callee(std::move(callee)), m_args(std::move(args)) {}

    Expr& GetCallee() { return *m_callee; }
    auto& GetArgs() { return m_args; }

  private:
    ExprUP m_callee;
    std::vector<ExprUP> m_args;
};
using CallUP = std::unique_ptr<Call>;

class Return : public ASTNode {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    Return (Token tok, ExprUP value)
        : ASTNode{tok}, m_value{std::move(value)}
    {}

    Expr& GetValue() { return *m_value; }

  private:
    ExprUP m_value;
};
using ReturnUP = std::unique_ptr<Return>;

class AST {
  public:
    AST(std::vector<ASTNodeUP> nodes) : m_nodes{std::move(nodes)} {}
    auto& GetNodes() { return m_nodes; }

  private:
    std::vector<ASTNodeUP> m_nodes;
};
}
