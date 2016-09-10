#pragma once

#include <unordered_map>
#include <string>
#include "PantsIsa.hpp"

namespace pants {
	std::unordered_map<std::string, Opcode> g_opcode_names{
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

	std::unordered_map<std::string, Register> g_register_names{
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
}