#ifndef RENDER_H
#define RENDER_H
#include <SDL3/SDL.h>
#include "HardWare.h"

SDL_Renderer* initRenderer();
void renderPixels(SDL_Renderer *renderer, Chip8 *chip);


#endif // RENDER_H