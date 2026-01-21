#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "OpCode.h"
#include "Render.h"




int main(int argc, char* argv[]) {
    Chip8 chip8;
    initChip8(&chip8);

    loadFontToChip(&chip8);
    
    //  "C:\\Temp\\Lab\\VisualStudio\\C\\ROMS\\15_Puzzle.ch8"  IBM_logo Airplane    Bowling [Gooitzen van der Wal]    Cave    Maze (alt) [David Winter, 199x]
    FILE* ptr = fopen("C:\\Temp\\Lab\\VisualStudio\\C\\ROMS\\Airplane.ch8", "rb");
    if (ptr == NULL) { printf("Error: Could not open ROM\n");  return 1; }
    size_t bytesRead = fread(&chip8.RAM[0x200], 1, (4096 - 0x200), ptr);
    fclose(ptr);

    
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_CreateWindowAndRenderer("Simple Pixel Draw", 640, 320, 0, &window, &renderer);
    SDL_SetRenderLogicalPresentation(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_LOGICAL_PRESENTATION_STRETCH);
        

    bool quit = false;
    SDL_Event e;
    bool debugMode = false;

    while (!quit) {

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) quit = true;

            handleKeyPress(debugMode , &chip8, &e);
        }

        for (int i = 0; i <100; i++) {
            fetchAndPrcocessOpCode(debugMode, &chip8);
        }

        renderPixels(renderer, &chip8);
      
        SDL_Delay(16);
    }


    SDL_Quit();
    return 0;

}