#pragma once

#include <unordered_map>
#include <string>

namespace pants {
	enum class Opcode {
		add_ = 0x00,
		sub_ = 0x01,
		mul_ = 0x02,
		div_ = 0x03,
		and_ = 0x04,
		or_ = 0x05,
		not_ = 0x06,
		copy_ = 0x07,
		load_ = 0x08,
		store_ = 0x09,
		set_ = 0x0a,
		call_ = 0x0b,
		jump_ = 0x0c,
		compare_ = 0x0d,
		branch_ = 0x0e,
		halt_ = 0x0f,
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