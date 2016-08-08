#pragma once

#include <memory>
#include <vector>

namespace pants {
	enum class BinOpSym {
		Add, Sub, Mul, Div, Lt, Le, Gt, Ge, Eq
	};

	enum class UnaryOpSym {
		Min, Not
	};

	class ASTNode {};

	using ASTNodeUP = std::unique_ptr<ASTNode>;

	class List : public ASTNode
	{
		std::vector<ASTNodeUP> m_elems;
	};

	class Id : public ASTNode {
		std::string m_id;
	};
	class FuncDecl : public ASTNode
	{
		Id m_name;
		List m_args;
		ASTNodeUP m_body;
	};

	class Quote : public ASTNode
	{
		ASTNodeUP m_form;
	};

	class For : public ASTNode
	{
		Id m_id;
		ASTNode m_list;
		ASTNode m_body;
	};

	class BinaryOp : public ASTNode
	{
		ASTNodeUP m_lhs;
		ASTNodeUP m_rhs;
		BinOpSym m_op;
	};

	class Let : public ASTNode
	{
		ASTNodeUP m_lhs;
		ASTNodeUP m_rhs;
	};

	class Cond : public ASTNode
	{
		std::vector<ASTNode> m_conds;
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

	class AST {
	public:
		AST(ASTNodeUP node) : m_node{ std::move(node) } {}
	private:
		ASTNodeUP m_node;
	};
}
