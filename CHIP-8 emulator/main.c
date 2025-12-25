#include <SDL3/SDL.h>
#include <stdio.h>
#include "HardWare.h"




int main(int argc, char* argv[]) {

    Chip8 chip8; 
    chip8.RAM[2] = 2;
    chip8.V[0] = chip8.RAM[2];
    printf("V0 holds : %d\n", chip8.V[0]);

    /*
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("CHIP-8 Test", 640, 480, 0);

    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Delay(3000);

    // Cleanup
    SDL_DestroyWindow(window);
    SDL_Quit();
    */

    return 0;
}