#include "HardWare.h"



uint8_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT] = { 0 };


void initChip8(Chip8* chip) {
    memset(chip, 0, sizeof(Chip8));
    chip->PC = 0x200;
}




void loadFontToChip(Chip8* chip)
{
    bit8 fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    for (int i = 0; i < sizeof(fontset); i++) {
        chip->RAM[FONT_ADDRESS_START + i] = fontset[i];
    }
}




void drawSprite(Chip8* chip, bit8 x, bit8 y, bit8 height) {
    bit16 spriteAddr = chip->I;

    for (int i = 0; i < height; i++) {
        bit8 spriteRow = chip->RAM[spriteAddr + i];

        chip->V[0xF] = 0;

        for (int bit = 0; bit < BYTE_SIZE; bit++) {

            bit8 pixel = (spriteRow >> (BYTE_SIZE - bit - 1)) & 0x1;

            if (pixel == 1) {
                int targetX = (x + bit) % 64;
                int targetY = (y + i) % 32;
                int index = targetX + (targetY * 64);

                if (chip->screen[index] == 1) {
                    chip->V[0xF] = 1;
                }

                chip->screen[index] ^= 1;
            }
        }
    }
}


void handleKeyPress(bool debugMode, Chip8* chip, SDL_Event* e)
{
    if (e->type == SDL_EVENT_KEY_DOWN) {
        if (debugMode) { printf("-------------- key pressed %02X---------------", e->key.key); }
        switch (e->key.key) {
        case SDLK_1: chip->keys[0x1] = 1; break;
        case SDLK_2: chip->keys[0x2] = 1; break;
        case SDLK_3: chip->keys[0x3] = 1; break;
        case SDLK_4: chip->keys[0xC] = 1; break;

        case SDLK_Q: chip->keys[0x4] = 1; break;
        case SDLK_W: chip->keys[0x5] = 1; break;
        case SDLK_E: chip->keys[0x6] = 1; break;
        case SDLK_R: chip->keys[0xD] = 1; break;

        case SDLK_A: chip->keys[0x7] = 1; break;
        case SDLK_S: chip->keys[0x8] = 1; break;
        case SDLK_D: chip->keys[0x9] = 1; break;
        case SDLK_F: chip->keys[0xE] = 1; break;

        case SDLK_Z: chip->keys[0xA] = 1; break;
        case SDLK_X: chip->keys[0x0] = 1; break;
        case SDLK_C: chip->keys[0xB] = 1; break;
        case SDLK_V: chip->keys[0xF] = 1; break;
        }
    }

    // Handle Key Up (Released)
    if (e->type == SDL_EVENT_KEY_UP) {
        switch (e->key.key) {
        case SDLK_1: chip->keys[0x1] = 0; break;
        case SDLK_2: chip->keys[0x2] = 0; break;
        case SDLK_3: chip->keys[0x3] = 0; break;
        case SDLK_4: chip->keys[0xC] = 0; break;

        case SDLK_Q: chip->keys[0x4] = 0; break;
        case SDLK_W: chip->keys[0x5] = 0; break;
        case SDLK_E: chip->keys[0x6] = 0; break;
        case SDLK_R: chip->keys[0xD] = 0; break;

        case SDLK_A: chip->keys[0x7] = 0; break;
        case SDLK_S: chip->keys[0x8] = 0; break;
        case SDLK_D: chip->keys[0x9] = 0; break;
        case SDLK_F: chip->keys[0xE] = 0; break;

        case SDLK_Z: chip->keys[0xA] = 0; break;
        case SDLK_X: chip->keys[0x0] = 0; break;
        case SDLK_C: chip->keys[0xB] = 0; break;
        case SDLK_V: chip->keys[0xF] = 0; break;
        }
    }
}