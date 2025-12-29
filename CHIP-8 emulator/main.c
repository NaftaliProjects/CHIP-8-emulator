#include <SDL3/SDL.h>
#include <stdio.h>
#include "HardWare.h"
#include "OpCode.h"



int main(int argc, char* argv[]) {

    Chip8 chip8; 
    initChip8(&chip8);
    loadTestOpcodeProgram(&chip8);
    for (int steps = 1; steps <= 9; steps++)
    {
        printf("address : 0x%03X , opcode 0x%04X \n", chip8.PC, chip8.opcode);
        fetch_opcode(&chip8);
        bool a = false;
        bool b = false;
        a = cond(chip8.opcode, &chip8);
        if (!a) { b = movePC(chip8.opcode, &chip8); }
        if (!b) { mathAndAssign(chip8.opcode, &chip8); }
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