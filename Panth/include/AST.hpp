#include <memory>

class ASTNode{};

using ASTNodeUP = std::unique_ptr<AstNode>;

class FuncDecl : public ASTNode
{};
class Quote : public ASTNode
{};
class For : public ASTNode
{};
class Operation : public ASTNode
{};

class Let : public ASTNode
{

};

class Cond : public ASTNode
{
    std::vector<AstNodeUP> m_conds;
};

class If : public ASTNode
{
private:
    ASTNodeUP m_cond;
    ASTNodeUP m_if;
    ASTNodeUP m_else;
};

class Call : public ASTNode
{
private:
    FuncDecl& m_func;
    std::vector<ASTNodeUP> m_args;
};
