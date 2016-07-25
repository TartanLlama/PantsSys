#include <iostream>
#include <array>
#include <utility>
#include <iterator>

#include <SDL2/SDL.h>

#include "PantsIsa.hpp"
#include "PantsUI.hpp"
#include "Config.hpp"

class RegisterSet {
public:
    uint64_t& r0() { return m_regs[static_cast<size_t>(Register::r0)]; }
    uint64_t& r1() { return m_regs[static_cast<size_t>(Register::r1)]; }
    uint64_t& r2() { return m_regs[static_cast<size_t>(Register::r2)]; }
    uint64_t& r3() { return m_regs[static_cast<size_t>(Register::r3)]; }
    uint64_t& r4() { return m_regs[static_cast<size_t>(Register::r4)]; }
    uint64_t& r5() { return m_regs[static_cast<size_t>(Register::r5)]; }
    uint64_t& r6() { return m_regs[static_cast<size_t>(Register::r6)]; }
    uint64_t& r7() { return m_regs[static_cast<size_t>(Register::r7)]; }
    uint64_t& r8() { return m_regs[static_cast<size_t>(Register::r8)]; }
    uint64_t& r9() { return m_regs[static_cast<size_t>(Register::r9)]; }
    uint64_t& r10() { return m_regs[static_cast<size_t>(Register::r10)]; }
    uint64_t& rs() { return m_regs[static_cast<size_t>(Register::rs)]; }
    uint64_t& sp() { return m_regs[static_cast<size_t>(Register::sp)]; }
    uint64_t& fp() { return m_regs[static_cast<size_t>(Register::fp)]; }
    uint64_t& pc() { return m_regs[static_cast<size_t>(Register::pc)]; }
    uint64_t& sn() { return m_regs[static_cast<size_t>(Register::sn)]; }
    uint64_t& me() { return m_regs[static_cast<size_t>(Register::me)]; }
    
    uint64_t& get (Register reg) { return m_regs[static_cast<size_t>(reg)]; }
    uint64_t& get (uint16_t reg) { return m_regs[static_cast<size_t>(reg)]; }

    const uint64_t& r0() const { return m_regs[static_cast<size_t>(Register::r0)]; }
    const uint64_t& r1() const { return m_regs[static_cast<size_t>(Register::r1)]; }
    const uint64_t& r2() const { return m_regs[static_cast<size_t>(Register::r2)]; }
    const uint64_t& r3() const { return m_regs[static_cast<size_t>(Register::r3)]; }
    const uint64_t& r4() const { return m_regs[static_cast<size_t>(Register::r4)]; }
    const uint64_t& r5() const { return m_regs[static_cast<size_t>(Register::r5)]; }
    const uint64_t& r6() const { return m_regs[static_cast<size_t>(Register::r6)]; }
    const uint64_t& r7() const { return m_regs[static_cast<size_t>(Register::r7)]; }
    const uint64_t& r8() const { return m_regs[static_cast<size_t>(Register::r8)]; }
    const uint64_t& r9() const { return m_regs[static_cast<size_t>(Register::r9)]; }
    const uint64_t& r10() const { return m_regs[static_cast<size_t>(Register::r10)]; }
    const uint64_t& rs() const { return m_regs[static_cast<size_t>(Register::rs)]; }
    const uint64_t& sp() const { return m_regs[static_cast<size_t>(Register::sp)]; }
    const uint64_t& fp() const { return m_regs[static_cast<size_t>(Register::fp)]; }
    const uint64_t& pc() const { return m_regs[static_cast<size_t>(Register::pc)]; }
    const uint64_t& sn() const { return m_regs[static_cast<size_t>(Register::sn)]; }
    const uint64_t& me() const { return m_regs[static_cast<size_t>(Register::me)]; }

    const uint64_t& get (Register reg) const { return m_regs[static_cast<size_t>(reg)]; }
    const uint64_t& get (uint16_t reg) const { return m_regs[static_cast<size_t>(reg)]; }

    void dump() { for (auto r : g_register_names) { std::cout << r.first << ' ' << get(r.second) << '\n'; } }
private:
    std::array<uint64_t, static_cast<size_t>(Register::n_registers)> m_regs;
};

using Memory = std::array<char, mem_size>;
using Instruction = std::pair<Opcode, uint32_t>;

uint16_t high16 (uint32_t i) {
    return i >> 16;
}

uint16_t low16 (uint32_t i) {
    return i & 0x00ff;
}

std::pair<Opcode, uint32_t> fetch (const RegisterSet& regs, const Memory& mem) {
    const char* pc = mem.data() + regs.pc();

    uint32_t opcode, operands;
    std::copy(pc, pc+4, reinterpret_cast<char*>(&opcode));
    std::copy(pc+4, pc+8, reinterpret_cast<char*>(&operands));
    
    Opcode op = static_cast<Opcode>(opcode);

    std::cout << "Decoded " << opcode << ' ' << operands << '\n';
    return {op, operands};
}

bool execute (const Instruction& inst, RegisterSet& regs, Memory& mem, PantsUI& ui) {
    Opcode op = inst.first;
    uint32_t operands = inst.second;
    
    switch (op) {
    case Opcode::addi:
        if (regs.sn()) 
            regs.rs() = uint64_t(int64_t(regs.get(high16(operands))) + int64_t(regs.get(low16(operands))));
        else
            regs.rs() = regs.get(high16(operands)) + regs.get(low16(operands));
        break;
    case Opcode::subi: regs.rs() = regs.get(high16(operands)) - regs.get(low16(operands)); break;
    case Opcode::muli:
        if (regs.sn()) 
            regs.rs() = uint64_t(int64_t(regs.get(high16(operands))) * int64_t(regs.get(low16(operands))));
        else
            regs.rs() = regs.get(high16(operands)) * regs.get(low16(operands));
        break;
        
    case Opcode::divi: regs.rs() = regs.get(high16(operands)) / regs.get(low16(operands)); break;
        
    case Opcode::setlowi: regs.rs() = (regs.rs() & (0xffffffff00000000)) + operands; break;
    case Opcode::sethighi: regs.rs() = (regs.rs() & (0x00000000ffffffff)) + (uint64_t(operands) << 32); break;

    case Opcode::comparei:
    {
        auto a = regs.get(high16(operands)), b = regs.get(low16(operands));
        if (a > b) regs.rs() = 2;
        else if (a < b) regs.rs() = 0;
        else regs.rs() = 1;

        break;
    }

    case Opcode::branchi:
        if (regs.get(high16(operands))) {
            regs.pc() = regs.me();
            return true;
        }
        break;

    case Opcode::storei: 
    {
        mem[regs.me()] = regs.get(low16(operands));
        if(regs.me() < vga_width * vga_height) {
            ui.redraw(mem);
        }
        break;
    }

    case Opcode::copyi: regs.get(high16(operands)) = regs.get(low16(operands)); break;
    case Opcode::jumpi: regs.pc() = regs.me(); return true;
        
    case Opcode::halti: return false;
    default: std::cerr << "Bad opcode" << static_cast<uint32_t>(op); abort(); break;
    }

    regs.pc() += 8;
    return true;
}

void setUpScreen() {
}

int main() {
    PantsUI ui{};

    RegisterSet regs{};
    Memory mem{};

    std::cin >> std::noskipws;
    std::istream_iterator<char> it (std::cin);
    std::istream_iterator<char> end;
    std::copy(it, end, mem.data());
    //for (int i = 0; i<64; ++i) std::cout << (int)mem[i] << ' ';

    while (true) {
        auto inst = fetch(regs, mem);
        if (!execute(inst, regs, mem, ui)) break;
        regs.dump();
    }

    SDL_Delay(2000);
}
