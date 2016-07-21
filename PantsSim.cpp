#include <iostream>

enum class Opcode {
    addi = 0x00,
    subi = 0x01,
    muli = 0x02,
    divi = 0x03,
    andi = 0x04,
    ori = 0x05,
    noti = 0x06,
    loadi = 0x07,
    storei = 0x08,
    setlowi = 0x09,
    sethighi = 0x0a,
    calli = 0x0b,
    reti = 0x0c,
    jumpi = 0x0d,
    comparei = 0x0e,
    branchi = 0x0f
};

enum class Register {
r0 = 0x00,
r1 = 0x01,
r2 = 0x02,
r3 = 0x03,
r4 = 0x04,
r5 = 0x05, 
r6 = 0x06,
r7 = 0x07,
r8 = 0x08,
r9 = 0x09,
r10 = 0x0a,
rs = 0x0b,
sp = 0x0c,
fp = 0x0d,
pc = 0x0e,
sn = 0x0f,
me = 0x10
};

uint16_t high16 (uint32_t i) {
    return i >> 16;
}

uint16_t low16 (uint32_t i) {
    return i & 0x00ff;
}

class Instruction {
    virtual void operator() = 0;
};

template <Opcode Op>
class InstImpl;

template<> class InstImpl<addi> : public Instruction
{
    Instruction(uint32_t ops) : 
        m_lhs{high16(ops)}, m_rhs{low16(ops)}
    {}

private:
    Register m_lhs, m_rhs;
};



std::unique_ptr<Instruction> decode (uint64_t inst) {
   Opcode opcode = inst >> 32;
   uint32_t operands = inst & 0x0000ffff;

   switch(opcode) {
   case Opcodes::addi: return std::make_unique<Instruction<addi>>(operands);
   default: return nullptr;
   }
}

Instruction fetch (std::istream& is) {
    uint64_t inst;
    is >> inst;
    return decode(inst);
}

int main() {

}
