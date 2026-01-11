#include <SDL3/SDL.h>
#include "OpCode.h"



int main(int argc, char* argv[]) {
    Chip8 chip8;
    initChip8(&chip8);
    loadTestOpcodeProgram(&chip8);

    FILE* ptr = fopen("C:\\Temp\\Lab\\VisualStudio\\C\\ROMS\\15_Puzzle.ch8", "rb");
    if (ptr == NULL) {
        printf("Error: Could not open ROM\n");
        return;
    }


    size_t bytesRead = fread(&chip8.RAM[0x200], 1, (4096 - 0x200), ptr);

    printf("Loaded %zu bytes into RAM starting at 0x200\n", bytesRead);
    fclose(ptr);


   
    for (int steps = 1; steps <= 100; steps++)
    {
        printf("address : 0x%03X , opcode 0x%04X \n", chip8.PC, chip8.opcode);
        fetchAndPrcocessOpCode(&chip8);
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