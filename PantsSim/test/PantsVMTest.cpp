#include "PantsSim.hpp"
#include "PantsUI.hpp"
#include "catch.hpp"

using namespace pants;
using namespace pants::sim;

using Code = std::initializer_list<uint16_t>;

void SetMem(Memory &mem, const Code &code) {
  std::copy(reinterpret_cast<const char *>(code.begin()),
            reinterpret_cast<const char *>(code.end()), begin(mem));
}

uint16_t reg2int(Register r) { return static_cast<uint16_t>(r); }

uint32_t make_operands(uint16_t a, uint16_t b) {
  uint32_t ret = b;
  return ret | (a << 16);
}

uint32_t make_operands(Register a, Register b) {
  return make_operands(reg2int(a), reg2int(b));
}

TEST_CASE("Instruction fetch", "[fetch]") {
  Memory mem{};
  RegisterSet regs{};

  SECTION("Add") {
    Code code = {0x00, 0x05, 0x00, 0x01};
    SetMem(mem, code);

    auto inst = fetch(regs, mem);

    REQUIRE(inst.opcode == Opcode::add_);
    REQUIRE(inst.target_reg == 0x05);
    REQUIRE(inst.operands == make_operands(0x01, 0x00));
  }

  SECTION("Halt") {
    Code code = {0x0f, 0x00, 0x00, 0x00};
    SetMem(mem, code);

    auto inst = fetch(regs, mem);

    REQUIRE(inst.opcode == Opcode::halt_);
    REQUIRE(inst.target_reg == 0x00);
    REQUIRE(inst.operands == 0x00);
  }
}

void test_set(Memory &mem, RegisterSet &regs, PantsUI &ui, uint32_t val) {
  Instruction set{Opcode::set_, reg2int(Register::r1), val};
  auto cont = execute(set, regs, mem, ui);
  REQUIRE(cont == true);
  REQUIRE(regs.r1() == val);
}

TEST_CASE("Execute", "[execute]") {
  Memory mem{};
  RegisterSet regs{};
  PantsUI ui{};

  SECTION("Add unsigned") {
    test_set(mem, regs, ui, 3);
    test_set(mem, regs, ui, 2);

    {
      Instruction add{Opcode::add_, reg2int(Register::r0),
                      make_operands(Register::r1, Register::r2)};
      auto cont = execute(add, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.r0() == 5);
    }
  }

  SECTION("Add signed") {
    {
      Instruction set{Opcode::set_, reg2int(Register::sn), 1};
      auto cont = execute(set, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.sn() == 1);
    }

    {
      Instruction set{Opcode::set_, reg2int(Register::r1), 3};
      auto cont = execute(set, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.r1() == 3);
    }

    {
      Instruction set{Opcode::set_, reg2int(Register::r2), -1};
      auto cont = execute(set, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.r2() == static_cast<uint32_t>(-1));
    }

    {
      Instruction add{Opcode::add_, reg2int(Register::r0),
                      make_operands(Register::r1, Register::r2)};
      auto cont = execute(add, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.r0() == 2);
    }
  }

  SECTION("Sub unsigned") {
    {
      Instruction set{Opcode::set_, reg2int(Register::r1), 3};
      auto cont = execute(set, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.r1() == 3);
    }

    {
      Instruction set{Opcode::set_, reg2int(Register::r2), 2};
      auto cont = execute(set, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.r2() == 2);
    }

    {
      Instruction sub{Opcode::sub_, reg2int(Register::r0),
                      make_operands(Register::r1, Register::r2)};
      auto cont = execute(sub, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.r0() == 1);
    }
  }

  SECTION("Sub signed") {
    {
      Instruction set{Opcode::set_, reg2int(Register::sn), 1};
      auto cont = execute(set, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.sn() == 1);
    }

    {
      Instruction set{Opcode::set_, reg2int(Register::r1), 3};
      auto cont = execute(set, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.r1() == 3);
    }

    {
      Instruction set{Opcode::set_, reg2int(Register::r2), -1};
      auto cont = execute(set, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.r2() == static_cast<uint32_t>(-1));
    }

    {
      Instruction sub{Opcode::sub_, reg2int(Register::r0),
                      make_operands(Register::r1, Register::r2)};
      auto cont = execute(sub, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.r0() == 4);
    }
  }

  SECTION("Mul unsigned") {
    {
      Instruction set{Opcode::set_, reg2int(Register::r1), 3};
      auto cont = execute(set, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.r1() == 3);
    }

    {
      Instruction set{Opcode::set_, reg2int(Register::r2), 2};
      auto cont = execute(set, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.r2() == 2);
    }

    {
      Instruction sub{Opcode::mul_, reg2int(Register::r0),
                      make_operands(Register::r1, Register::r2)};
      auto cont = execute(sub, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.r0() == 6);
    }
  }

  SECTION("Mul signed") {
    {
      Instruction set{Opcode::set_, reg2int(Register::sn), 1};
      auto cont = execute(set, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.sn() == 1);
    }

    {
      Instruction set{Opcode::set_, reg2int(Register::r1), 3};
      auto cont = execute(set, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.r1() == 3);
    }

    {
      Instruction set{Opcode::set_, reg2int(Register::r2), -1};
      auto cont = execute(set, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.r2() == static_cast<uint32_t>(-1));
    }

    {
      Instruction mul{Opcode::mul_, reg2int(Register::r0),
                      make_operands(Register::r1, Register::r2)};
      auto cont = execute(mul, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.r0() == -3);
    }
  }

  SECTION("Div unsigned") {
    {
      Instruction set{Opcode::set_, reg2int(Register::r1), 3};
      auto cont = execute(set, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.r1() == 3);
    }

    {
      Instruction set{Opcode::set_, reg2int(Register::r2), 2};
      auto cont = execute(set, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.r2() == 2);
    }

    {
      Instruction div{Opcode::div_, reg2int(Register::r0),
                      make_operands(Register::r1, Register::r2)};
      auto cont = execute(div, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.r0() == 1);
    }
  }

  SECTION("Div signed") {
    {
      Instruction set{Opcode::set_, reg2int(Register::sn), 1};
      auto cont = execute(set, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.sn() == 1);
    }

    {
      Instruction set{Opcode::set_, reg2int(Register::r1), 3};
      auto cont = execute(set, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.r1() == 3);
    }

    {
      Instruction set{Opcode::set_, reg2int(Register::r2), -1};
      auto cont = execute(set, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.r2() == static_cast<uint32_t>(-1));
    }

    {
      Instruction div{Opcode::div_, reg2int(Register::r0),
                      make_operands(Register::r1, Register::r2)};
      auto cont = execute(div, regs, mem, ui);
      REQUIRE(cont == true);
      REQUIRE(regs.r0() == -3);
    }
  }
}
