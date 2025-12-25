#pragma once
//config

typedef unsigned char  bit8;
typedef unsigned short bit16; 

bit16 const MAX_ADDRESS = 0xFFF;
bit16 const MIN_ADDRESS = 0x000;

bit16 const MAX_RESEVED_FOR_DISPLAY_ADDRESS = 0xFFF;
bit16 const MIN_RESREVED_FOR_DISPLAY_ADDRESS = 0xF00;

bit16 const MAX_OTHER_RESERVED_ADDRESS = 0xEFF;
bit16 const MIN_OTHER_RESERVED_ADDRESS = 0xEA0;

bit16 const MAX_INTERPRETER_ADDRESS = 0x1FF;
bit16 const MIN_INTERPRETER_ADDRESS = 0x000;


typedef struct {
    bit8  V[16];     // V0-VF registers
    bit16 PC;        // Program Counter
    bit16 I;         // Index Register
    bit8  RAM[4096]; // Memory

    // Some systems also have timers
    bit8  delay_timer;
    bit8  sound_timer;
} Chip8;






