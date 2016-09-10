#pragma once

#include <unordered_map>
#include <string>

namespace pants {
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

	extern std::unordered_map<std::string, Opcode> g_opcode_names;
	
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
	extern std::unordered_map<std::string, Register> g_register_names;
}