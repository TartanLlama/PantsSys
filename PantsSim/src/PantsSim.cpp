#include <array>
#include <iostream>
#include <iterator>
#include <utility>

#include "BitTwiddling.hpp"
#include "Config.hpp"
#include "PantsIsa.hpp"
#include "PantsSim.hpp"
#include "PantsUI.hpp"

using namespace pants::util;

namespace pants {
namespace sim {
void RegisterSet::dump() {
    for (auto r : g_register_names) {
        std::cout << r.first << ' ' << get(r.second) << '\n';
    }
}

Instruction fetch(const RegisterSet &regs, const Memory &mem) {
    const char *pc = mem.data() + regs.pc();

    Instruction inst{};
    std::copy(pc, (pc + sizeof(uint16_t)),
              reinterpret_cast<char *>(&inst.opcode));
    pc += sizeof(uint16_t);
    std::copy(pc, (pc + sizeof(uint16_t)),
              reinterpret_cast<char *>(&inst.target_reg));
    pc += sizeof(uint16_t);
    std::copy(pc, (pc + sizeof(uint32_t)),
              reinterpret_cast<char *>(&inst.operands));
    return inst;
}

bool execute(const Instruction &inst, RegisterSet &regs, Memory &mem,
             PantsUI &ui) {
    Opcode op = inst.opcode;
    uint16_t target = inst.target_reg;
    uint32_t operands = inst.operands;

    switch (op) {
    case Opcode::add_:
        if (regs.sn())
            regs.get(target) = uint32_t(int32_t(regs.get(high16(operands))) +
                                        int32_t(regs.get(low16(operands))));
        else
            regs.get(target) =
                regs.get(high16(operands)) + regs.get(low16(operands));
        break;
    case Opcode::sub_:
        if (regs.sn())
            regs.get(target) = uint32_t(int32_t(regs.get(high16(operands))) -
                                        int32_t(regs.get(low16(operands))));
        else
            regs.get(target) =
                regs.get(high16(operands)) - regs.get(low16(operands));
        break;
    case Opcode::mul_:
        if (regs.sn())
            regs.get(target) = uint32_t(int32_t(regs.get(high16(operands))) *
                                        int32_t(regs.get(low16(operands))));
        else
            regs.get(target) =
                regs.get(high16(operands)) * regs.get(low16(operands));
        break;

    case Opcode::div_:
        if (regs.sn())
            regs.get(target) = uint32_t(int32_t(regs.get(high16(operands))) /
                                        int32_t(regs.get(low16(operands))));
        else
            regs.get(target) =
                regs.get(high16(operands)) / regs.get(low16(operands));
        break;
    case Opcode::set_:
        regs.get(target) = operands;
        break;
    case Opcode::compare_: {
        auto a = regs.get(high16(operands)), b = regs.get(low16(operands));
        if (a > b)
            regs.get(target) = 2;
        else if (a < b)
            regs.get(target) = 0;
        else
            regs.get(target) = 1;

        break;
    }

    case Opcode::branch_:
        if (regs.get(high16(operands))) {
            regs.pc() = regs.me() + code_offset;
            return true;
        }
        break;

    case Opcode::store_: {
        auto ptr = mem.data() + regs.me();
        auto val = regs.get(low16(operands));

        std::copy(reinterpret_cast<unsigned char *>(&val),
                  reinterpret_cast<unsigned char *>(&val) + sizeof(val), ptr);
        if (regs.me() < vga_width * vga_height) {
            ui.redraw(mem);
        }
        break;
    }

    case Opcode::copy_:
        regs.get(high16(operands)) = regs.get(low16(operands));
        break;
    case Opcode::call_:
        regs.ra() = regs.pc() + 8; // FALLTHROUGH
    case Opcode::jump_:
        regs.pc() = regs.me() + code_offset;
        return true;

    case Opcode::halt_:
        return false;
    default:
        std::cerr << "Bad opcode" << static_cast<uint16_t>(op);
        abort();
        break;
    }

    regs.pc() += 8;
    return true;
}
}
}
