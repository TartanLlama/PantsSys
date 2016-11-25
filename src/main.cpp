#include "PantsVM.hpp"
#include "PantsUI.hpp"
#include <iostream>

using namespace pants::emu;
int main() {
	try {
		PantsUI ui{};

		RegisterSet regs{};
		regs.pc() = code_offset;
		Memory mem{};

		std::cin >> std::noskipws;
		std::istream_iterator<char> it(std::cin);
		std::istream_iterator<char> end;
		std::copy(it, end, mem.data() + code_offset);
		//    for (int i = 0; i<64; ++i) std::cout << (int)mem[i+code_offset] << ' ';

		while (true) {
			auto inst = fetch(regs, mem);
			if (!execute(inst, regs, mem, ui)) break;
			regs.dump();
		}

		SDL_Delay(2000);
	}
	catch (std::runtime_error& err) {
		std::cerr << err.what();
		return -1;
	}
	return 0;
}
