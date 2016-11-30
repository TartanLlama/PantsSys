#pragma once

#include <type_traits>

template <typename T> struct DependentFalse : std::false_type {};