#pragma once

#include <memory>
#include <vector>

#include "Tokens.hpp"
#include "Type.hpp"

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
class Assign;
class If;
class Return;
class BinaryOp;
class Call;
class UnaryOp;
class Type;

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
    virtual void Visit(Assign &) = 0;
    virtual void Visit(If &) = 0;
    virtual void Visit(Return &) = 0;
    virtual void Visit(BinaryOp &) = 0;
    virtual void Visit(Call &) = 0;
    virtual void Visit(UnaryOp &) = 0;
    virtual void Visit(Type &) = 0;
};

class ASTNode {
  public:
    virtual void Accept(ASTVisitor &visitor) = 0;
    explicit ASTNode(Token tok) : m_tok{tok} {}
    Token Tok() { return m_tok; }

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
    std::string String() { return m_tok.String().value(); }
};

class Int : public Expr {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    Int(Token tok) : Expr{tok} {}
    int Val() { return m_tok.Int().value(); }
};

class Bool : public Expr {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    Bool(Token tok) : Expr{tok} {}
};

    
class BinaryOp : public Expr {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    BinaryOp(Token tok, ASTNodeUP lhs, ASTNodeUP rhs, Token op)
        : Expr{tok}, m_lhs{std::move(lhs)}, m_rhs{std::move(rhs)}, m_op{op} {}

    ASTNode &Lhs() { return *m_lhs; }
    ASTNode &Rhs() { return *m_rhs; }
    Token Tok() { return m_op; }

  private:
    ASTNodeUP m_lhs;
    ASTNodeUP m_rhs;
    Token m_op;
};

class UnaryOp : public Expr {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    ASTNodeUP m_lhs;
    ASTNodeUP m_rhs;
    Token m_op;
};

class Assign : public ASTNode {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    ASTNodeUP lhs;
    ASTNodeUP rhs;
};

class Type : public ASTNode {
public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    Type(Token tok) : ASTNode{tok} {}
};


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
    ASTNode* GetInit() { return m_init.get(); }
    
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

    Id Name() { return m_name; }
    Type& GetType() { return m_type; }
    auto ParamsBegin() { return begin(m_params); }
    auto ParamsEnd() { return end(m_params); }
    auto BodyBegin() { return begin(m_body); }
    auto BodyEnd() { return end(m_body); }

  private:
    Id m_name;
    Type m_type;
    std::vector<VarDeclUP> m_params;
    std::vector<ASTNodeUP> m_body;
};

class ClassDecl : public ASTNode {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }

  private:
    Id m_name;
    std::vector<VarDeclUP> m_vars;
};

class EnumDecl : public ASTNode {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    Id m_name;
    std::vector<Id> m_enums;
};

class For : public ASTNode {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    Id m_id;
    ASTNodeUP m_range;
    std::vector<ASTNodeUP> m_body;
};

class While : public ASTNode {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    While (Token tok, ExprUP cond, std::vector<ASTNodeUP> body)
        : ASTNode(tok), m_cond(std::move(cond)), m_body(std::move(body)) {}

    Expr& Cond() { return *m_cond; }
    auto BodyBegin() { return m_body.begin(); }
    auto BodyEnd() { return m_body.end(); }
    
private:
    ExprUP m_cond;
    std::vector<ASTNodeUP> m_body;
};

class If : public ASTNode {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }

  private:
    ASTNodeUP m_cond;
    std::vector<ASTNodeUP> m_if;
    std::vector<ASTNodeUP> m_else;
};

class Call : public Expr {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    Call (Token tok, ExprUP callee, std::vector<ExprUP> args)
        : Expr(tok), m_callee(std::move(callee)), m_args(std::move(args)) {}

    Expr& Callee() { return *m_callee; }
    auto ArgsBegin() { return m_args.begin(); }
    auto ArgsEnd() { return m_args.end(); }

  private:
    ExprUP m_callee;
    std::vector<ExprUP> m_args;
};

class Return : public ASTNode {
  public:
    void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
    Return (Token tok, ExprUP value)
        : ASTNode{tok}, m_value{std::move(value)}
    {}

    Expr& Value() { return *m_value; }
    
  private:
    ExprUP m_value;
};

class AST {
  public:
    AST(ASTNodeUP node) : m_node{std::move(node)} {}
    ASTNode &Root() { return *m_node; }

  private:
    ASTNodeUP m_node;
};
}
