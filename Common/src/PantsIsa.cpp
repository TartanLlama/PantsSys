#include "PantsIsa.hpp"
#include <string>
#include <unordered_map>
#include <stdexcept>

namespace pants {
Register RegisterFromName (const std::string& reg) {
    return g_register_names.at(reg);
}

std::unordered_map<std::string, Register> g_register_names{
    {"r0", Register::r0}, {"r1", Register::r1},   {"r2", Register::r2},
    {"r3", Register::r3}, {"r4", Register::r4},   {"r5", Register::r5},
    {"r6", Register::r6}, {"r7", Register::r7},   {"r8", Register::r8},
    {"r9", Register::r9}, {"r10", Register::r10}, {"rs", Register::rs},
    {"sp", Register::sp}, {"fp", Register::fp},   {"pc", Register::pc},
    {"sn", Register::sn}, {"me", Register::me},   {"ra", Register::ra},
};


std::string NameFromRegister (Register reg) {
    switch (reg) {
    case Register::r0: return "r0";
    case Register::r1: return "r1";
    case Register::r2: return "r2";
    case Register::r3: return "r3";
    case Register::r4: return "r4";
    case Register::r5: return "r5";
    case Register::r6: return "r6";
    case Register::r7: return "r7";
    case Register::r8: return "r8";
    case Register::r9: return "r9";
    case Register::r10: return "r10";
    case Register::rs: return "rs";
    case Register::sp: return "sp";
    case Register::fp: return "fp";
    case Register::pc: return "pc";
    case Register::sn: return "sn";
    case Register::me: return "me";
    case Register::ra: return "ra";
    default: throw std::runtime_error("Unhandled register");
    }
}

std::unordered_map<std::string, Opcode> g_opcode_names{
    {"add", Opcode::add_},       {"sub", Opcode::sub_},
    {"mul", Opcode::mul_},       {"div", Opcode::div_},
    {"and", Opcode::and_},       {"or", Opcode::or_},
    {"not", Opcode::not_},       {"copy", Opcode::copy_},
    {"load", Opcode::load_},     {"store", Opcode::store_},
    {"set", Opcode::set_},       {"jump", Opcode::jump_},
    {"call", Opcode::call_},     {"compare", Opcode::compare_},
    {"branch", Opcode::branch_}, {"halt", Opcode::halt_},
};

}
