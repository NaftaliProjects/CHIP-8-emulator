#include "HardWare.h"



uint8_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT] = { 0 };


void initChip8(Chip8* chip) {
    chip->PC = 0x200;

    chip->I = 0;
    chip->SP = 0;
    chip->opcode = 0;

    for (int i = 0; i < 4096; i++) {
        chip->RAM[i] = 0;
    }

    for (int i = 0; i < 16; i++) {
        chip->V[i] = 0;
    }

    for (int i = 0; i < 16; i++) {
        chip->stack[i] = 0;
    }

    memset(chip->screen, 0, sizeof(chip->screen));
    memset(chip->keys, 0, sizeof(chip->keys));

    chip->delay_timer = 0;
    chip->sound_timer = 0;

}


bool fetch_opcode(Chip8* chip) {
    if (chip->PC >= MIN_OTHER_RESERVED_ADDRESS - 1) {
        printf("Error: Program Counter reached reserved memory or out of bounds at 0x%03X\n", chip->PC);
        return false;
    }

    chip->opcode = (chip->RAM[chip->PC] << 8) | chip->RAM[chip->PC + 1];
    

    chip->PC += 2;

    return true;
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
    printf("DEBUG: drawSprite called at (%d, %d) with height %d\n", x, y, height);
    bit16 spriteAddr = chip->I;

    for (int i = 0; i < height; i++) {
        bit8 spriteRow = chip->RAM[spriteAddr + i];
        printf("DEBUG: Drawing Sprite Row %d. Address: 0x%03X, Value: 0x%02X\n", i, spriteAddr + i, spriteRow);

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