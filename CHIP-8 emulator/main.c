#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "OpCode.h"
#include "Render.h"




int main(int argc, char* argv[]) {
    Chip8 chip8;
    initChip8(&chip8);

    loadFontToChip(&chip8);

    FILE* ptr = fopen("C:\\Temp\\Lab\\VisualStudio\\C\\ROMS\\15_Puzzle.ch8", "rb");
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
            if (e.type == SDL_EVENT_QUIT) quit = true;

            // Handle Key Down (Pressed)
            if (e.type == SDL_EVENT_KEY_DOWN) {
                if (e.key.key == SDLK_ESCAPE) quit = true;
                printf("-------------- key pressed %02X---------------", e.key.key);
                switch (e.key.key) {
                    case SDLK_1: chip8.keys[0x1] = 1; break;
                    case SDLK_2: chip8.keys[0x2] = 1; break;
                    case SDLK_3: chip8.keys[0x3] = 1; break;
                    case SDLK_4: chip8.keys[0xC] = 1; break;

                    case SDLK_Q: chip8.keys[0x4] = 1; break;
                    case SDLK_W: chip8.keys[0x5] = 1; break;
                    case SDLK_E: chip8.keys[0x6] = 1; break;
                    case SDLK_R: chip8.keys[0xD] = 1; break;

                    case SDLK_A: chip8.keys[0x7] = 1; break;
                    case SDLK_S: chip8.keys[0x8] = 1; break;
                    case SDLK_D: chip8.keys[0x9] = 1; break;
                    case SDLK_F: chip8.keys[0xE] = 1; break;

                    case SDLK_Z: chip8.keys[0xA] = 1; break;
                    case SDLK_X: chip8.keys[0x0] = 1; break;
                    case SDLK_C: chip8.keys[0xB] = 1; break;
                    case SDLK_V: chip8.keys[0xF] = 1; break;
                }
            }

            // Handle Key Up (Released)
            if (e.type == SDL_EVENT_KEY_UP) {
                switch (e.key.key) {
                    case SDLK_1: chip8.keys[0x1] = 0; break;
                    case SDLK_2: chip8.keys[0x2] = 0; break;
                    case SDLK_3: chip8.keys[0x3] = 0; break;
                    case SDLK_4: chip8.keys[0xC] = 0; break;

                    case SDLK_Q: chip8.keys[0x4] = 0; break;
                    case SDLK_W: chip8.keys[0x5] = 0; break;
                    case SDLK_E: chip8.keys[0x6] = 0; break;
                    case SDLK_R: chip8.keys[0xD] = 0; break;

                    case SDLK_A: chip8.keys[0x7] = 0; break;
                    case SDLK_S: chip8.keys[0x8] = 0; break;
                    case SDLK_D: chip8.keys[0x9] = 0; break;
                    case SDLK_F: chip8.keys[0xE] = 0; break;

                    case SDLK_Z: chip8.keys[0xA] = 0; break;
                    case SDLK_X: chip8.keys[0x0] = 0; break;
                    case SDLK_C: chip8.keys[0xB] = 0; break;
                    case SDLK_V: chip8.keys[0xF] = 0; break;
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
        SDL_Delay(300);
    }


    SDL_Quit();
    return 0;

}