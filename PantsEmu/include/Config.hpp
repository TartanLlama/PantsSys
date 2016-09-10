#pragma once

#include <cstdint>

namespace pants {
	namespace emu {
		constexpr size_t mem_size = 1024 * 512;

		constexpr size_t screen_width = 640;
		constexpr size_t screen_height = 480;

		constexpr size_t vga_width = screen_width / 10;
		constexpr size_t vga_height = screen_height / 10;
		constexpr size_t vga_size = vga_width * vga_height;

		constexpr size_t code_offset = vga_size;

		const std::string font_path = "C:\\\\Users\\Simon\\Documents/PantsSys/PantsVM/assets/Inconsolata-g.ttf";
		constexpr size_t font_size = 12;
	}
}