#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "OpCode.h"
#include "Render.h"
#include <errno.h> 



//  "C:\\Temp\\Lab\\VisualStudio\\C\\ROMS\\15_Puzzle.ch8"  IBM_logo Airplane    Bowling [Gooitzen van der Wal]    Cave    Maze (alt) [David Winter, 199x]

int main(int argc, char* argv[]) {
    

    bool debugMode = true;
    bool quit = false;

    Chip8 chip8;
    initChip8(&chip8);
    loadFontToChip(debugMode ,&chip8);
    
    //load ROM
    FILE* ptr = fopen("C:\\Temp\\Lab\\VisualStudio\\C\\ROMS\\Airplane.ch8", "rb");
    if (!loadRom(debugMode, &chip8, ptr)) { return errno; }

    SDL_Renderer* renderer = initRenderer();
    Uint64 lastTimerTick = SDL_GetTicks();

    SDL_Event e;
    

    while (!quit) {

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) quit = true;

            handleKeyPress(debugMode , &chip8, &e);
        }

        for (int i = 0; i <10; i++) {
            fetchAndPrcocessOpCode(debugMode, &chip8);
        }

        Uint32 now = SDL_GetTicks();
        if (now - lastTimerTick >= 16) {
            if (chip8.delay_timer > 0)
                chip8.delay_timer--;

            if (chip8.sound_timer > 0)
                chip8.sound_timer--;

            lastTimerTick = now;
        }

        renderPixels(renderer, &chip8);
        SDL_Delay(60);   


    }


    SDL_Quit();
    return 0;

}