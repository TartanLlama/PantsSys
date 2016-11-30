#pragma once

#include "Config.hpp"
#include "PantsIsa.hpp"
#include <array>
#include <utility>

namespace pants {
namespace sim {
class PantsUI;

class RegisterSet {
public:
  uint32_t &r0() { return m_regs[static_cast<size_t>(Register::r0)]; }
  uint32_t &r1() { return m_regs[static_cast<size_t>(Register::r1)]; }
  uint32_t &r2() { return m_regs[static_cast<size_t>(Register::r2)]; }
  uint32_t &r3() { return m_regs[static_cast<size_t>(Register::r3)]; }
  uint32_t &r4() { return m_regs[static_cast<size_t>(Register::r4)]; }
  uint32_t &r5() { return m_regs[static_cast<size_t>(Register::r5)]; }
  uint32_t &r6() { return m_regs[static_cast<size_t>(Register::r6)]; }
  uint32_t &r7() { return m_regs[static_cast<size_t>(Register::r7)]; }
  uint32_t &r8() { return m_regs[static_cast<size_t>(Register::r8)]; }
  uint32_t &r9() { return m_regs[static_cast<size_t>(Register::r9)]; }
  uint32_t &r10() { return m_regs[static_cast<size_t>(Register::r10)]; }
  uint32_t &rs() { return m_regs[static_cast<size_t>(Register::rs)]; }
  uint32_t &sp() { return m_regs[static_cast<size_t>(Register::sp)]; }
  uint32_t &fp() { return m_regs[static_cast<size_t>(Register::fp)]; }
  uint32_t &pc() { return m_regs[static_cast<size_t>(Register::pc)]; }
  uint32_t &sn() { return m_regs[static_cast<size_t>(Register::sn)]; }
  uint32_t &me() { return m_regs[static_cast<size_t>(Register::me)]; }
  uint32_t &ra() { return m_regs[static_cast<size_t>(Register::ra)]; }

  uint32_t &get(Register reg) { return m_regs[static_cast<size_t>(reg)]; }
  uint32_t &get(uint16_t reg) { return m_regs[static_cast<size_t>(reg)]; }

  const uint32_t &r0() const {
    return m_regs[static_cast<size_t>(Register::r0)];
  }
  const uint32_t &r1() const {
    return m_regs[static_cast<size_t>(Register::r1)];
  }
  const uint32_t &r2() const {
    return m_regs[static_cast<size_t>(Register::r2)];
  }
  const uint32_t &r3() const {
    return m_regs[static_cast<size_t>(Register::r3)];
  }
  const uint32_t &r4() const {
    return m_regs[static_cast<size_t>(Register::r4)];
  }
  const uint32_t &r5() const {
    return m_regs[static_cast<size_t>(Register::r5)];
  }
  const uint32_t &r6() const {
    return m_regs[static_cast<size_t>(Register::r6)];
  }
  const uint32_t &r7() const {
    return m_regs[static_cast<size_t>(Register::r7)];
  }
  const uint32_t &r8() const {
    return m_regs[static_cast<size_t>(Register::r8)];
  }
  const uint32_t &r9() const {
    return m_regs[static_cast<size_t>(Register::r9)];
  }
  const uint32_t &r10() const {
    return m_regs[static_cast<size_t>(Register::r10)];
  }
  const uint32_t &rs() const {
    return m_regs[static_cast<size_t>(Register::rs)];
  }
  const uint32_t &sp() const {
    return m_regs[static_cast<size_t>(Register::sp)];
  }
  const uint32_t &fp() const {
    return m_regs[static_cast<size_t>(Register::fp)];
  }
  const uint32_t &pc() const {
    return m_regs[static_cast<size_t>(Register::pc)];
  }
  const uint32_t &sn() const {
    return m_regs[static_cast<size_t>(Register::sn)];
  }
  const uint32_t &me() const {
    return m_regs[static_cast<size_t>(Register::me)];
  }
  const uint32_t &ra() const {
    return m_regs[static_cast<size_t>(Register::ra)];
  }

  const uint32_t &get(Register reg) const {
    return m_regs[static_cast<size_t>(reg)];
  }
  const uint32_t &get(uint16_t reg) const {
    return m_regs[static_cast<size_t>(reg)];
  }

  void dump();

private:
  std::array<uint32_t, static_cast<size_t>(Register::n_registers)> m_regs;
};

using Memory = std::array<char, mem_size>;

struct Instruction {
  Opcode opcode;
  uint16_t target_reg;
  uint32_t operands;
};

Instruction fetch(const RegisterSet &regs, const Memory &mem);
bool execute(const Instruction &inst, RegisterSet &regs, Memory &mem,
             PantsUI &ui);
}
}
