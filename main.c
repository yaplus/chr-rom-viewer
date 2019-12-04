#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "rom.h"

const int SCREEN_WIDTH  = 256;
const int SCREEN_HEIGHT = 224;

const uint8_t PALLETTE[] = { 0x00, 0x55, 0xAA, 0xFF };

void draw_chr_rom(SDL_Renderer *renderer, uint8_t* chr_rom, unsigned int length)
{
    int sprites = length / 16;
    int s, l, i;
    for (s = 0; s < sprites; ++s) {
        for (l = 0; l < 8; ++l) {
            for (i = 0; i < 8; ++i) {
                uint8_t low  =  (chr_rom[l + s*16]     >> i) & 1;
                uint8_t high = ((chr_rom[l + 8 + s*16] >> i) & 1) << 1;
                uint8_t color = low | high;
                SDL_SetRenderDrawColor(renderer, PALLETTE[color], PALLETTE[color], PALLETTE[color], 0xFF);
                int x = (s%(SCREEN_WIDTH/8))*8;
                int y = (s/(SCREEN_WIDTH/8))*8;
                SDL_RenderDrawPoint(renderer, x + 8 - i, y + l);
            }
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        perror("argument");
        return 1;
    }

    const char* filename = argv[1];
    if (rom_init(filename) == ROM_LOAD_FAILED) {
        perror("NES ROM Loading");
        return 1;
    }

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
        filename,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_OPENGL
    );
    if (window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create window: %s", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_SOFTWARE
    );
    if (renderer == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create renderer: %s", SDL_GetError());
        return 1;
    }

    while (1) {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        draw_chr_rom(renderer, ROM_CHR, rom_get_size_chr());
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}