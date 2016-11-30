#pragma once
#include <cstdint>

namespace pants {
namespace util {
/// Extracts the high 16 bits from a 32 bit unsigned integer
inline uint16_t high16(uint32_t i) { return i >> 16; }

/// Extracts the low 16 bits from a 32 bit unsigned integer
inline uint16_t low16(uint32_t i) { return i & 0x00ff; }

/// Extracts the high 8 bits from a 16 bit unsigned integer
inline uint8_t high8(uint16_t i) { return i >> 8; }

/// Extracts the low 8 bits from a 16 bit unsigned integer
inline uint8_t low8(uint16_t i) { return i & 0x0f; }
}
}
