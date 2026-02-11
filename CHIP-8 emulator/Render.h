#ifndef RENDER_H
#define RENDER_H
#include <SDL3/SDL.h>
#include "HardWare.h"

SDL_Renderer* initRenderer(int screenWidth, int screenHeight);
void renderPixels(SDL_Renderer* renderer, Chip8* chip, int screenWidth, int screenHeight);


#endif // RENDER_H