#include <iostream>
#include <array>
#include <utility>
#include <iterator>

#include "PantsIsa.hpp"
#include "PantsUI.hpp"
#include "Config.hpp"
#include "PantsVM.hpp"

namespace pants {
	namespace emu {
		void RegisterSet::dump(){ for (auto r : g_register_names) { std::cout << r.first << ' ' << get(r.second) << '\n'; } }

		uint16_t high16(uint32_t i) {
			return i >> 16;
		}

		uint16_t low16(uint32_t i) {
			return i & 0x00ff;
		}

		std::pair<Opcode, uint32_t> fetch(const RegisterSet& regs, const Memory& mem) {
			const char* pc = mem.data() + regs.pc();

			uint32_t opcode, operands;
			std::copy(pc, pc + 4, reinterpret_cast<char*>(&opcode));
			std::copy(pc + 4, pc + 8, reinterpret_cast<char*>(&operands));

			Opcode op = static_cast<Opcode>(opcode);

			return{ op, operands };
		}

		bool execute(const Instruction& inst, RegisterSet& regs, Memory& mem, PantsUI& ui) {
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
					regs.pc() = regs.me() + code_offset;
					return true;
				}
				break;

			case Opcode::storei:
			{
				auto ptr = mem.data() + regs.me();
				auto val = regs.get(low16(operands));
				std::copy(reinterpret_cast<unsigned char*>(&val), reinterpret_cast<unsigned char*>(&val) + sizeof(decltype(val)), ptr);
				if (regs.me() < vga_width * vga_height) {
					ui.redraw(mem);
				}
				break;
			}

			case Opcode::copyi: regs.get(high16(operands)) = regs.get(low16(operands)); break;
			case Opcode::jumpi: regs.pc() = regs.me() + code_offset; return true;

			case Opcode::halti: return false;
			default: std::cerr << "Bad opcode" << static_cast<uint32_t>(op); abort(); break;
			}

			regs.pc() += 8;
			return true;
		}
	}
}
