#include "catch.hpp"
#include "PantsVM.hpp"

using namespace pants;
using namespace pants::emu;

using Code = std::initializer_list<uint32_t>;

void SetMem (Memory& mem, const Code& code) {
		std::copy(reinterpret_cast<const char*>(code.begin()),
			reinterpret_cast<const char*>(code.end()),
			begin(mem));
}

TEST_CASE("Instruction fetch", "[fetch]") {
	Memory mem{};
	RegisterSet regs{};
	
	SECTION("Add") {
		Code code = { 0x00, 0x01 };
		SetMem(mem, code);

		Opcode opcode;
		uint32_t operands;
		std::tie(opcode, operands) = fetch(regs, mem);

		REQUIRE(opcode == Opcode::addi);
		REQUIRE(operands == 0x01);
	}

	SECTION("Halt") {
		Code code = { 0x0f, 0x00 };
		SetMem(mem, code);

		Opcode opcode;
		uint32_t operands;
		std::tie(opcode, operands) = fetch(regs, mem);

		REQUIRE(opcode == Opcode::halti);
		REQUIRE(operands == 0x00);
	}
}

TEST_CASE("Execute", "[execute]") {
	REQUIRE(1==1);
}