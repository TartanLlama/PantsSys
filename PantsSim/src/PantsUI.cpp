#include <iostream>
#include <array>

#include "PantsUI.hpp"
#include "PantsEnc.hpp"

namespace pants {
	namespace sim {
		void PantsUI::redraw(const std::array<char, mem_size>& mem) {
			SDL_Color white = { 0,0,0 };
			SDL_Color black = { 0xff,0xff,0xff };

			SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
			SDL_RenderClear(m_renderer);


			for (size_t i = 0; i < vga_width * vga_height; ++i) {
				//std::cout << (uint32_t)mem[i] << '\n';
				char text[2] = { decode_table[mem[i]], '\0' };
				if (text[0] != '\0') {
					SDL_Surface* text_surface = TTF_RenderText_Blended(m_font, text, black);
					if (!text_surface) {
						throw std::runtime_error{ std::string{"Unable to render text surface: "}.append(TTF_GetError()) };
					}

					m_texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
					if (!m_texture) {
						throw std::runtime_error{ std::string{"Unable to create texture from rendered text: "}.append(TTF_GetError()) };
					}

					SDL_FreeSurface(text_surface);

					int tw, th;
					SDL_QueryTexture(m_texture, nullptr, nullptr, &tw, &th);
					SDL_Rect render_quad = { int(i * 6), 0, tw, th };
					SDL_RenderCopy(m_renderer, m_texture, nullptr, &render_quad);
				}
			}

			SDL_RenderPresent(m_renderer);
		}
	}
}
