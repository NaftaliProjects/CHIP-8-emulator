#ifndef HARWARE_H
#define HARWARE_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h> 
#include <SDL3/SDL.h>

typedef uint8_t  bit8;
typedef uint16_t bit16;

#define BYTE_SIZE 8

#define MAX_ADDRESS 0xFFF
#define MIN_ADDRESS 0x000

#define MAX_RESEVED_FOR_DISPLAY_ADDRESS 0xFFF
#define MIN_RESREVED_FOR_DISPLAY_ADDRESS 0xF00

#define MAX_OTHER_RESERVED_ADDRESS 0xEFF
#define MIN_OTHER_RESERVED_ADDRESS 0xEA0

#define MAX_INTERPRETER_ADDRESS 0x1FF
#define MIN_INTERPRETER_ADDRESS 0x000

#define FONT_ADDRESS_START 0x000
#define SIZE_OF_FONT_INSTANCE 5


#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32


typedef struct {
    bit8  V[16];        //  V0-VF
    bit16 PC;           // Program Counter 
    bit16 I;            // Index Register 


    //(Stack)
    bit16 stack[16];    
    bit8  SP;           // Stack Pointer 

    bit8  RAM[4096];   

    bit8 screen[SCREEN_WIDTH * SCREEN_HEIGHT];
    bit8  delay_timer;
    bit8  sound_timer;

    bit8 keys[16];

    bit16 opcode;      
} Chip8;


void initChip8(Chip8* chip);
bool fetch_opcode(Chip8* chip);
void loadFontToChip(Chip8* chip);

void drawSprite(Chip8* chip, bit8 x, bit8 y, bit8 height);
void handleKeyPress(Chip8* chip, SDL_Event* e);

#endif // HARWARE_H