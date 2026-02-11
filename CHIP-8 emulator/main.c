#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "OpCode.h"
#include "Render.h"
#include <errno.h> 


// Particle Demo [zeroZshadow, 2008]      Stars [Sergey Naydenov, 2010]       Hires Worm V4 [RB-Revival Studios, 2007]
//  "C:\\Temp\\Lab\\VisualStudio\\C\\ROMS\\15_Puzzle.ch8"  IBM_logo     Airplane    Bowling [Gooitzen van der Wal]    Cave    Maze (alt) [David Winter, 199x]

int main(int argc, char* argv[]) {
    

    bool debugMode = true;
    bool quit = false;

    Chip8 chip8 = { 0 };
    initChip8(&chip8);
    loadFontToChip(debugMode ,&chip8);
    
    //load ROM
    FILE* ptr = fopen("C:\\Temp\\Lab\\VisualStudio\\C\\ROMS\\Hires Worm V4 [RB-Revival Studios, 2007].ch8", "rb");
    if (!loadRom(debugMode, &chip8, ptr)) { return errno; }

    SDL_Renderer* renderer = initRenderer(chip8.config.screenWidth, chip8.config.screenHeight);
    Uint64 lastTimerTick = SDL_GetTicks();

    SDL_Event e;
    

    int clockSpeed = chip8.config.clockSpeed;
    int targetFPS = chip8.config.targetFPS;
    int ticksPerFrame = clockSpeed / targetFPS;


    while (!quit) {
        Uint32 startTick = SDL_GetTicks();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) quit = true;
            handleKeyPress(debugMode, &chip8, &e);
        }

     
        for (int i = 0; i < ticksPerFrame; i++) {
            fetchAndPrcocessOpCode(debugMode, &chip8);
        }

        
        if (chip8.delay_timer > 0) chip8.delay_timer--;
        if (chip8.sound_timer > 0) chip8.sound_timer--;

       
        renderPixels(renderer, &chip8, chip8.config.screenWidth, chip8.config.screenHeight);

       
        Uint32 frameTicks = SDL_GetTicks() - startTick;
        if (frameTicks < 1000 / targetFPS) {
            SDL_Delay((1000 / targetFPS) - frameTicks);
        }
    }


    SDL_Quit();
    return 0;

}