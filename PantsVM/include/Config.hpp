#pragma once

#include <cstdint>

constexpr size_t mem_size = 1024*1024*5;

constexpr size_t screen_width = 640;
constexpr size_t screen_height = 480;

constexpr size_t vga_width = screen_width/10;
constexpr size_t vga_height = screen_height/10;

const std::string font_path = "/home/simon/play/PantsSys/PantsVM/assets/Inconsolata-g.ttf";
constexpr size_t font_size = 10;
