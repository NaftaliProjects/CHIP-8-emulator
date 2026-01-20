#ifndef RENDER_H
#define RENDER_H
#include <SDL3/SDL.h>
#include "HardWare.h"

void renderPixels(SDL_Renderer *renderer, Chip8 *chip);
//void setPixelWhite(int x, int y);

#endif // RENDER_H