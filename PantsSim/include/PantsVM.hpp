#pragma once

#include <array>
#include <utility>
#include "PantsIsa.hpp"
#include "Config.hpp"

namespace pants {
	namespace emu {
		class PantsUI;

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

			uint64_t& get(Register reg) { return m_regs[static_cast<size_t>(reg)]; }
			uint64_t& get(uint16_t reg) { return m_regs[static_cast<size_t>(reg)]; }

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

			const uint64_t& get(Register reg) const { return m_regs[static_cast<size_t>(reg)]; }
			const uint64_t& get(uint16_t reg) const { return m_regs[static_cast<size_t>(reg)]; }

			void dump();
		private:
			std::array<uint64_t, static_cast<size_t>(Register::n_registers)> m_regs;
		};

		using Memory = std::array<char, mem_size>;
		using Instruction = std::pair<Opcode, uint32_t>;

		std::pair<Opcode, uint32_t> fetch(const RegisterSet& regs, const Memory& mem);
		bool execute(const Instruction& inst, RegisterSet& regs, Memory& mem, PantsUI& ui);
	}
}