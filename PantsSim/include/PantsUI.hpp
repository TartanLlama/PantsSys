#pragma once

#include <stdexcept>

#define SDL_MAIN_HANDLED

#if defined(__linux__) || defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#elif _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#endif


#include "Config.hpp"
#include <iostream>

namespace pants {
namespace sim {
class PantsUI {
  public:
    ~PantsUI() {
        if (m_font) {
            TTF_CloseFont(m_font);
        }
        if (m_renderer) {
            SDL_DestroyRenderer(m_renderer);
        }
        if (m_window) {
            SDL_DestroyWindow(m_window);
        }
        if (m_texture) {
            SDL_DestroyTexture(m_texture);
        }

        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }

    PantsUI(bool dummy = false)
        : m_window(nullptr), m_surface(nullptr),
          m_font(nullptr), m_renderer(nullptr), m_texture(nullptr) {
        if (dummy) return;

        SDL_Init(SDL_INIT_VIDEO);

        m_window = SDL_CreateWindow("PantsSim", SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED, screen_width,
                                    screen_height, SDL_WINDOW_SHOWN);
        if (!m_window) {
            std::cerr << SDL_GetError();
            throw std::runtime_error{
                std::string{"Unable to create window: "}.append(
                    SDL_GetError())};
        }

        m_surface = SDL_GetWindowSurface(m_window);
        if (!m_surface) {
            throw std::runtime_error{
                std::string{"Unable to get window surface: "}.append(
                    SDL_GetError())};
        }

        m_renderer = SDL_GetRenderer(m_window);
        if (!m_renderer) {
            m_renderer = SDL_CreateRenderer(m_window, -1, 0);
            if (!m_renderer) {
                throw std::runtime_error{
                    std::string{"Unable to get renderer: "}.append(
                        SDL_GetError())};
            }
        }

        auto img_flags = IMG_INIT_PNG;
        if (!(IMG_Init(img_flags) & img_flags)) {
            throw std::runtime_error{
                std::string{"Unable to init SDL_image: "}.append(
                    IMG_GetError())};
        }

        if (TTF_Init() == -1) {
            throw std::runtime_error{
                std::string{"Unable to init SDL_ttf: "}.append(TTF_GetError())};
        }

        m_font = TTF_OpenFont(font_path.c_str(), font_size);
        if (!m_font) {
            throw std::runtime_error{
                std::string{"Unable to load font: "}.append(TTF_GetError())};
        }

        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
        SDL_RenderClear(m_renderer);

        SDL_RenderPresent(m_renderer);
    }

    void redraw(const std::array<char, mem_size> &mem);

  private:
    SDL_Window *m_window;
    SDL_Surface *m_surface;
    TTF_Font *m_font;
    SDL_Renderer *m_renderer;
    SDL_Texture *m_texture;
};
}
}
