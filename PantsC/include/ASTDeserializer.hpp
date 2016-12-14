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

template<> auto ASTDeserializer::DeserializeNode<ASTNode>() -> ASTNodeUP;
template<> auto ASTDeserializer::DeserializeNode<Expr>() -> ExprUP;
template<> auto ASTDeserializer::DeserializeNode<Id>() -> IdUP;
template<> auto ASTDeserializer::DeserializeNode<Int>() -> IntUP;
template<> auto ASTDeserializer::DeserializeNode<Bool>() -> BoolUP;
template<> auto ASTDeserializer::DeserializeNode<Type>() -> TypeUP;
template<> auto ASTDeserializer::DeserializeNode<VarDecl>() -> VarDeclUP;
template<> auto ASTDeserializer::DeserializeNode<FuncDecl>() -> FuncDeclUP;
template<> auto ASTDeserializer::DeserializeNode<ClassDecl>() -> ClassDeclUP;
template<> auto ASTDeserializer::DeserializeNode<EnumDecl>() -> EnumDeclUP;
template<> auto ASTDeserializer::DeserializeNode<For>() -> ForUP;
template<> auto ASTDeserializer::DeserializeNode<While>() -> WhileUP;
template<> auto ASTDeserializer::DeserializeNode<If>() -> IfUP;
template<> auto ASTDeserializer::DeserializeNode<Return>() -> ReturnUP;
template<> auto ASTDeserializer::DeserializeNode<BinaryOp>() -> BinaryOpUP;
template<> auto ASTDeserializer::DeserializeNode<Call>() -> CallUP;
template<> auto ASTDeserializer::DeserializeNode<UnaryOp>() -> UnaryOpUP;
}
