#pragma once

namespace pants {
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

template <typename T> constexpr uint32_t id_for = IdFor<T>::value;
}
