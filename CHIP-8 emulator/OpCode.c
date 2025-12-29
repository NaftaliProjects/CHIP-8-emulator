#include "OpCode.h"
#include <stdio.h>

bool movePC(bit16 opcode, Chip8* chip)
{
    if ((opcode & 0xF000) == 0x0000) {
        if (opcode == 0x00EE) {
            // RETURN: 
            chip->PC = chip->stack[--chip->SP];
            printf("opcode = Return from stack\n");
            return true;
        }
        if (opcode == 0x00E0) {
            // CLEAR SCREEN: 
            printf("opcode = Clear Screen\n");
            return true;
        }
        return true;
    }

    bit8 op = (opcode & 0xF000) >> 12;
    bit16 address = (opcode & 0x0FFF);

    switch (op) {
    case 0x1: // Jump 1NNN
        chip->PC = address;
        printf("opcode = Jump 1NNN\n");
        break;

    case 0x2: // Call 2NNN
        chip->stack[chip->SP] = chip->PC;
        chip->SP++;
        chip->PC = address;
        printf("opcode = Call 2NNN\n");
        break;

    case 0xB: // Jump + V0
        chip->PC = address + chip->V[0];
        printf("opcode = Jump + V0\n");
        break;

    default:
        return false;
    }
    return true;
}