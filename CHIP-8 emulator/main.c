#include <SDL3/SDL.h>
#include <stdio.h>
#include "HardWare.h"


bool fetch_opcode(Chip8* chip) {
    if (chip->PC >= MIN_OTHER_RESERVED_ADDRESS) return false;

    chip->opcode = (chip->RAM[chip->PC] << 8) | chip->RAM[chip->PC + 1];
    chip->PC += 2;

    return true;
}

int main(int argc, char* argv[]) {

    Chip8 chip8; 
    chip8.PC = 0x000;
    for (bit16 i = 0x000; i <= 40; i++) {
        chip8.RAM[i] = 1;
    }

    for (bit16 i = 0x000; i <= 20; i++) {
        fetch_opcode(&chip8);
        printf("opcode is: %d\n", chip8.opcode);
    }
    
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