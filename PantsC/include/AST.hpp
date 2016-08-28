#pragma once

#include <memory>
#include <vector>

#include "Type.hpp"

namespace pants {
	enum class BinOpSym {
		Add, Sub, Mul, Div, Lt, Le, Gt, Ge, Eq
	};

	enum class UnaryOpSym {
		Min, Not
	};

	class ASTNode {
		Token m_tok;
	};

	using ASTNodeUP = std::unique_ptr<ASTNode>;

	class Id : public ASTNode {
	};

	class VarDecl : public ASTNode {
		Id m_type;
		Id m_id;
	};
	using VarDeclUP = std::unique_ptr<VarDecl>;

	class FuncDecl : public ASTNode
	{
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

	class BinaryOp : public ASTNode
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
