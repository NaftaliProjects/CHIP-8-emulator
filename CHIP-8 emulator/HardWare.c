#include "HardWare.h"



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

void loadTestOpcodeProgram(Chip8* chip) {
    uint16_t start_addr = 0x200;


    bit8 test_program[] = {
        0x60, 0x01, // 6001: Set V0 to 0x01
        0x61, 0x02, // 6102: Set V1 to 0x02
        0x80, 0x14, // 8014: V0 = V0 + V1 (V0 should become 0x03)
        0x30, 0x03, // 3003: Skip next if V0 == 0x03 (Should skip)
        0x12, 0x00, // 1200: Jump to 0x200 (If skip fails, we loop here)
        0x62, 0xFF, // 62FF: Set V2 to 0xFF (We only get here if skip worked)
        0xA5, 0x55, // A555: Set Index Register I to 0x555
        0x12, 0x0A  // 120A: Jump to 0x20A (Infinite loop at the end of test)
    };

    // העתקת התוכנית לתוך ה-RAM של האמולטור
    for (int i = 0; i < sizeof(test_program); i++) {
        chip->RAM[start_addr + i] = test_program[i];
    }
}