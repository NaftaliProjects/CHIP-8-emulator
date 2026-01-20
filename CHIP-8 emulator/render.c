#include "Render.h"





void renderPixels(SDL_Renderer* renderer, Chip8 *chip) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            if (chip->screen[y * SCREEN_WIDTH + x] == 1) {
                SDL_FRect rect = { (float)x, (float)y, 1.0f, 1.0f };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

