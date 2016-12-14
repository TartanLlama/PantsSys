#pragma once

#include "AST.hpp"
#include "Tokens.hpp"
#include "ASTSerializationIds.hpp"
#include "UniquePtrCast.hpp"
#include "Assert.hpp"

#include <iostream>
#include <type_traits>
#include <vector>
#include <algorithm>

namespace {
template <typename> struct tag{};
}

namespace pants {
class ASTDeserializer {
public:
    ASTDeserializer(std::istream& is) : m_is{is} {}
    AST Deserialize();

private:
    uint32_t PeekTag() {
        uint32_t tag;
        m_is.read(reinterpret_cast<char*>(&tag), sizeof(uint32_t));
        m_is.seekg(-sizeof(uint32_t), std::ios_base::cur);
        return tag;
    }

    template <typename T>
    T Read(tag<T>) {
        T t;
        m_is.read(reinterpret_cast<char*>(&t), sizeof(T));
        return t;
    }

    std::string Read(tag<std::string>) {
        std::string s;
        std::getline(m_is, s, '\0');
        return s;
    }

    Token Read(tag<Token>) {
        std::underlying_type_t<Token::Kind> k_val;
        std::size_t row, col;
        m_is.read(reinterpret_cast<char*>(&k_val), sizeof(k_val));
        m_is.read(reinterpret_cast<char*>(&row), sizeof(row));
        m_is.read(reinterpret_cast<char*>(&col), sizeof(col));

        Token::Kind k = static_cast<Token::Kind>(k_val);

        if (k == Token::id_) return {k,row,col,Read<std::string>()};
        if (k == Token::int_) return {k,row,col,Read<int>()};
        return {k,row,col};
    }

    template <typename T>
    std::vector<std::unique_ptr<T>> Read(tag<std::vector<T>>) {
        auto count = Read<size_t>();
        std::vector<std::unique_ptr<T>> vec{};
        vec.reserve(count);
        for (size_t i = 0; i < count; ++i) {
            vec.push_back(DeserializeNode<T>());
        }
        return vec;
    }

    template <typename T>
    auto Read() -> decltype(Read(tag<T>{})){
        return Read(tag<T>{});
    }

    template <typename T>
    std::unique_ptr<T> DeserializeNode();

    std::istream& m_is;
};

template<>
std::unique_ptr<ASTNode> ASTDeserializer::DeserializeNode<ASTNode>();

template<>
std::unique_ptr<Expr> ASTDeserializer::DeserializeNode<Expr>();


template<>
inline std::unique_ptr<Id> ASTDeserializer::DeserializeNode<Id>() {
    auto id = Read<uint32_t>();
    ASSERT(id == id_for<Id>);
    auto tok = Read<Token>();
    return std::make_unique<Id>(tok);
}

template<>
inline std::unique_ptr<Int> ASTDeserializer::DeserializeNode<Int>() {
    auto id = Read<uint32_t>();
    ASSERT(id == id_for<Int>);
    auto tok = Read<Token>();
    return std::make_unique<Int>(tok);
}

template<>
inline std::unique_ptr<Bool> ASTDeserializer::DeserializeNode<Bool>() {
    auto id = Read<uint32_t>();
    ASSERT(id == id_for<Bool>);
    auto tok = Read<Token>();
    return std::make_unique<Bool>(tok);
}

template<>
inline std::unique_ptr<Type> ASTDeserializer::DeserializeNode<Type>() {
    auto id = Read<uint32_t>();
    ASSERT(id == id_for<Type>);
    auto tok = Read<Token>();

    return std::make_unique<Type>(tok);
}

template<>
inline std::unique_ptr<VarDecl> ASTDeserializer::DeserializeNode<VarDecl>() {
    auto id = Read<uint32_t>();
    ASSERT(id == id_for<VarDecl>);
    auto tok = Read<Token>();

    auto type = *DeserializeNode<Type>();
    auto name = *DeserializeNode<Id>();

    auto has_init = Read<bool>();
    if (has_init) {
        auto init = DeserializeNode<Expr>();
        return std::make_unique<VarDecl>(tok,std::move(type),std::move(name),std::move(init));
    }

    return std::make_unique<VarDecl>(tok,std::move(type),std::move(name));
}

template<>
inline std::unique_ptr<FuncDecl> ASTDeserializer::DeserializeNode<FuncDecl>() {
    auto id = Read<uint32_t>();
    ASSERT(id == id_for<FuncDecl>);
    auto tok = Read<Token>();

    auto name = *DeserializeNode<Id>();
    auto type = *DeserializeNode<Type>();

    auto params = Read<std::vector<VarDecl>>();
    auto body = Read<std::vector<ASTNode>>();

    return std::make_unique<FuncDecl>(tok,std::move(name),std::move(type),std::move(params),std::move(body));
}

template<>
inline std::unique_ptr<ClassDecl> ASTDeserializer::DeserializeNode<ClassDecl>() {
    auto id = Read<uint32_t>();
    ASSERT(id == id_for<ClassDecl>);
    auto tok = Read<Token>();
    auto name = *DeserializeNode<Id>();
    auto vars = Read<std::vector<VarDecl>>();

    return std::make_unique<ClassDecl>(tok,std::move(name),std::move(vars));
}

template<>
inline std::unique_ptr<EnumDecl> ASTDeserializer::DeserializeNode<EnumDecl>() {
    auto id = Read<uint32_t>();
    ASSERT(id == id_for<EnumDecl>);
    auto tok = Read<Token>();

    auto enums = pointer_container_get_values(Read<std::vector<Id>>());
    auto name = *DeserializeNode<Id>();
    return std::make_unique<EnumDecl>(tok,std::move(name),std::move(enums));
}

template<>
inline std::unique_ptr<For> ASTDeserializer::DeserializeNode<For>() {
    auto id = Read<uint32_t>();
    ASSERT(id == id_for<For>);
    auto tok = Read<Token>();

    auto name = *DeserializeNode<Id>();
    auto range = DeserializeNode<ASTNode>();
    auto body = Read<std::vector<ASTNode>>();

    return std::make_unique<For>(tok, name, std::move(range), std::move(body));
}

template<>
inline std::unique_ptr<While> ASTDeserializer::DeserializeNode<While>() {
    auto id = Read<uint32_t>();
    ASSERT(id == id_for<While>);
    auto tok = Read<Token>();

    auto cond = DeserializeNode<Expr>();
    auto body = Read<std::vector<ASTNode>>();

    return std::make_unique<While>(tok, std::move(cond), std::move(body));
}

template<>
inline std::unique_ptr<If> ASTDeserializer::DeserializeNode<If>() {
    auto id = Read<uint32_t>();
    ASSERT(id == id_for<If>);
    auto tok = Read<Token>();

    auto n_conds = Read<size_t>();
    std::vector<If::CondBodyPair> conds;
    conds.reserve(n_conds);
    for (size_t i = 0; i < n_conds; ++i) {
        auto cond = DeserializeNode<Expr>();

        auto n_statements = Read<size_t>();
        std::vector<ASTNodeUP> body;
        body.reserve(n_statements);
        for (size_t j = 0; j < n_statements; ++j) {
            body.push_back(DeserializeNode<ASTNode>());
        }

        conds.push_back(std::make_pair(std::move(cond),std::move(body)));
    }

    auto else_body = Read<std::vector<ASTNode>>();

    return std::make_unique<If>(tok, std::move(conds), std::move(else_body));
}

template<>
inline std::unique_ptr<Return> ASTDeserializer::DeserializeNode<Return>() {
    auto id = Read<uint32_t>();
    ASSERT(id == id_for<Return>);
    auto tok = Read<Token>();
    auto expr = DeserializeNode<Expr>();

    return std::make_unique<Return>(tok,std::move(expr));
}

template<>
inline std::unique_ptr<BinaryOp> ASTDeserializer::DeserializeNode<BinaryOp>() {
    auto id = Read<uint32_t>();
    ASSERT(id == id_for<BinaryOp>);
    auto tok = Read<Token>();
    auto lhs = DeserializeNode<Expr>();
    auto rhs = DeserializeNode<Expr>();
    auto op = Read<Token>();

    return std::make_unique<BinaryOp>(tok, std::move(lhs), std::move(rhs), op);
}

template<>
inline std::unique_ptr<Call> ASTDeserializer::DeserializeNode<Call>() {
    auto id = Read<uint32_t>();
    ASSERT(id == id_for<Call>);
    auto tok = Read<Token>();
    auto callee = DeserializeNode<Expr>();
    auto args = Read<std::vector<Expr>>();

    return std::make_unique<Call>(tok, std::move(callee), std::move(args));
}

template<>
inline std::unique_ptr<UnaryOp> ASTDeserializer::DeserializeNode<UnaryOp>() {
    auto id = Read<uint32_t>();
    ASSERT(id == id_for<UnaryOp>);
    auto tok = Read<Token>();
    auto arg = DeserializeNode<Expr>();
    auto op = Read<Token>();

    return std::make_unique<UnaryOp>(tok, std::move(arg), op);

}

template<>
inline ASTNodeUP ASTDeserializer::DeserializeNode<ASTNode>() {
    auto id = PeekTag();

    switch (id) {
    case id_for<Id>: return DeserializeNode<Id>();
    case id_for<Int>: return DeserializeNode<Int>();
    case id_for<Bool>: return DeserializeNode<Bool>();
    case id_for<VarDecl>: return DeserializeNode<VarDecl>();
    case id_for<FuncDecl>: return DeserializeNode<FuncDecl>();
    case id_for<ClassDecl>: return DeserializeNode<ClassDecl>();
    case id_for<EnumDecl>: return DeserializeNode<EnumDecl>();
    case id_for<For>: return DeserializeNode<For>();
    case id_for<While>: return DeserializeNode<While>();
    case id_for<Expr>: return DeserializeNode<Expr>();
    case id_for<If>: return DeserializeNode<If>();
    case id_for<Return>: return DeserializeNode<Return>();
    case id_for<BinaryOp>: return DeserializeNode<BinaryOp>();
    case id_for<Call>: return DeserializeNode<Call>();
    case id_for<UnaryOp>: return DeserializeNode<UnaryOp>();
    case id_for<Type>: return DeserializeNode<Type>();
    default: throw std::runtime_error("Can't deserialize base case node");
    }
}

template<>
inline ExprUP ASTDeserializer::DeserializeNode<Expr>() {
    auto id = PeekTag();

    switch (id) {
    case id_for<Id>: return DeserializeNode<Id>();
    case id_for<Int>: return DeserializeNode<Int>();
    case id_for<Bool>: return DeserializeNode<Bool>();
    case id_for<Expr>: return DeserializeNode<Expr>();
    case id_for<BinaryOp>: return DeserializeNode<BinaryOp>();
    case id_for<Call>: return DeserializeNode<Call>();
    case id_for<UnaryOp>: return DeserializeNode<UnaryOp>();
    default: throw std::runtime_error("Not an expression");
    }
}
}
