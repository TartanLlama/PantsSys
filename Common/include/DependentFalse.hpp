#pragma once

#include <type_traits>

/// A dependent false template class for use in unconditional static_asserts
///
/// static_assert(DependentFalse<T>::value, "message");
template <typename T> struct DependentFalse : std::false_type {};
