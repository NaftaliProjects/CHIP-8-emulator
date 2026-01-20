#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "OpCode.h"
#include "Render.h"




int main(int argc, char* argv[]) {
    Chip8 chip8;
    initChip8(&chip8);
    //loadTestOpcodeProgram(&chip8);
    loadFontToChip(&chip8);

    FILE* ptr = fopen("C:\\Temp\\Lab\\VisualStudio\\C\\ROMS\\Airplane.ch8", "rb");
    if (ptr == NULL) {
        printf("Error: Could not open ROM\n");
        return 1;
    }


    size_t bytesRead = fread(&chip8.RAM[0x200], 1, (4096 - 0x200), ptr);

    printf("Loaded %zu bytes into RAM starting at 0x200\n", bytesRead);
    for (int i = 0; i < 15; i++) {
        int addr = 0x200 + (i * 2);

        // Check we aren't printing beyond what we actually read
        if (addr + 1 < 0x200 + bytesRead) {
            printf("%02X%02X ", chip8.RAM[addr], chip8.RAM[addr + 1]);
        }
    }
    fclose(ptr);


   


    
    
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_CreateWindowAndRenderer("Simple Pixel Draw", 640, 320, 0, &window, &renderer);

 
    SDL_SetRenderLogicalPresentation(renderer, SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_LOGICAL_PRESENTATION_STRETCH);
  





    bool quit = false;
    SDL_Event e;

    while (!quit) {

        while (SDL_PollEvent(&e)) {
            printf("Event detected! Type: 0x%08X\n", e.type);

            if (e.type == SDL_EVENT_QUIT) {
                printf("Quit event received (0x%08X)\n", e.type);
                quit = true;
            }

            if (e.type == SDL_EVENT_KEY_DOWN) {   
                printf("Key Pressed: 0x%08X\n", e.key.key);

                if (e.key.key == SDLK_ESCAPE) {
                    quit = true;
                }
            }
        }

        for (int i = 0; i <10; i++) {
            fetchAndPrcocessOpCode(true, &chip8);
        }

        int activePixels = 0;
        for (int i = 0; i < 64 * 32; i++) {
            if (chip8.screen[i] == 1) activePixels++;
        }
        printf("DEBUG: Before rendering - Active pixels in memory: %d\n", activePixels);

        renderPixels(renderer, &chip8);

        printf("Event detected! Type: 0x%08X\n", e.type);
        SDL_Delay(16);
    }


    SDL_Quit();
    return 0;

}