#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Config.hpp"

class PantsUI {
public:
    ~PantsUI() {
        TTF_CloseFont(m_font);

        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_DestroyTexture(m_texture);

        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }
    
    PantsUI() : m_window(nullptr), m_surface(nullptr) {
        SDL_Init(SDL_INIT_VIDEO);
        
        m_window = SDL_CreateWindow("PantsEmu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
        if (!m_window) {
            throw std::runtime_error{std::string{"Unable to create window: "}.append(SDL_GetError())};
        }

        m_surface = SDL_GetWindowSurface(m_window);
        if (!m_surface) {
            throw std::runtime_error{std::string{"Unable to get window surface: "}.append(SDL_GetError())};
        }

        m_renderer = SDL_GetRenderer(m_window);
        if (!m_renderer) {
            throw std::runtime_error{std::string{"Unable to get renderer: "}.append(SDL_GetError())};
        }

        auto img_flags = IMG_INIT_PNG;
        if (! (IMG_Init(img_flags) & img_flags) ) {
            throw std::runtime_error{std::string{"Unable to init SDL_image: "}.append(IMG_GetError())};
        }

        if ( TTF_Init() == -1 ) {
            throw std::runtime_error{std::string{"Unable to init SDL_ttf: "}.append(TTF_GetError())};
        }

        m_font = TTF_OpenFont(font_path.c_str(), font_size);
        if (!m_font) {
            throw std::runtime_error{std::string{"Unable to load font: "}.append(TTF_GetError())};
        }

        SDL_Color colour = {100,110,110};
        SDL_Surface* text_surface = TTF_RenderText_Solid(m_font, "E", colour);
        if (!text_surface) {
            throw std::runtime_error{std::string{"Unable to render text surface: "}.append(TTF_GetError())};
        }

        m_texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
        if (!m_texture) {
            throw std::runtime_error{std::string{"Unable to create texture from rendered text: "}.append(TTF_GetError())};
        }
        
        SDL_FreeSurface(text_surface);

        SDL_SetRenderDrawColor( m_renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( m_renderer );
        
        SDL_Rect render_quad = { 0, 0, 10, 10 };
        SDL_RenderCopy(m_renderer, m_texture, nullptr, &render_quad);

        SDL_RenderPresent(m_renderer);

    }

    void redraw(const std::array<char, mem_size>& mem) {

    }

private:
    SDL_Window* m_window;
    SDL_Surface* m_surface;
    TTF_Font* m_font;
    SDL_Renderer* m_renderer;
    SDL_Texture* m_texture;
};
