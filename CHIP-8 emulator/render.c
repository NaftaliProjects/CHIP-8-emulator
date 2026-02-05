#include "Render.h"




SDL_Renderer* initRenderer()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_CreateWindowAndRenderer("Simple Pixel Draw", 640, 320, 0, &window, &renderer);
    SDL_SetRenderLogicalPresentation(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_LOGICAL_PRESENTATION_STRETCH);

    return renderer;
}

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

