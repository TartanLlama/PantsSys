#pragma once

#include <unordered_map>
#include <string>

enum class Opcode {
    addi = 0x00,
        subi = 0x01,
        muli = 0x02,
        divi = 0x03,
        andi = 0x04,
        ori = 0x05,
        noti = 0x06,
        copyi = 0x07,
        loadi = 0x08,
        storei = 0x09,
        setlowi = 0x0a,
        sethighi = 0x0b,
        jumpi = 0x0c,
        comparei = 0x0d,
        branchi = 0x0e,
        halti = 0x0f,
        n_opcodes
        };

std::unordered_map<std::string,Opcode> g_opcode_names {
    {"add", Opcode::addi},
    {"sub", Opcode::subi},
    {"mul", Opcode::muli},
    {"div", Opcode::divi},
    {"and", Opcode::andi},
    {"or", Opcode::ori},
    {"not", Opcode::noti},
    {"copy", Opcode::copyi},
    {"load", Opcode::loadi},
    {"store", Opcode::storei},
    {"setlow", Opcode::setlowi},
    {"sethigh", Opcode::sethighi},
    {"jump", Opcode::jumpi},
    {"compare", Opcode::comparei},
    {"branch", Opcode::branchi},
    {"halt", Opcode::halti},
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
        me = 0x10,
        ra = 0x11,
        n_registers
        };

std::unordered_map<std::string, Register> g_register_names {
    {"r0", Register::r0},
    {"r1", Register::r1},
    {"r2", Register::r2},
    {"r3", Register::r3},
    {"r4", Register::r4},
    {"r5", Register::r5},
    {"r6", Register::r6},
    {"r7", Register::r7},
    {"r8", Register::r8},
    {"r9", Register::r9},
    {"r10", Register::r10},
    {"rs", Register::rs},
    {"sp", Register::sp},
    {"fp", Register::fp},
    {"pc", Register::pc},
    {"sn", Register::sn},
    {"me", Register::me},
    {"ra", Register::ra},
};
