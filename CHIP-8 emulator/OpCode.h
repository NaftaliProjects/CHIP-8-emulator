#pragma once
#include "HardWare.h";
bool movePC(bit16 opcode);



bool movePC(bit16 opcode, Chip8* chip)
{
    if ((opcode & 0xF000) == 0x0000) {
        if (opcode == 0x00EE) {
            // RETURN: 
            chip->PC = chip->stack[--chip->SP];
            return true;
        }
        if (opcode == 0x00E0) {
            // CLEAR SCREEN: 
            return true;
        }
        return true;
    }

    bit8 op = (opcode & 0xF000) >> 12;
    bit16 address = (opcode & 0x0FFF);

    switch (op) {
    case 0x1: // Jump 1NNN
        chip->PC = address;
        break;

    case 0x2: // Call 2NNN
        chip->stack[chip->SP] = chip->PC;
        chip->SP++;
        chip->PC = address;
        break;

    case 0xB: // Jump + V0
        chip->PC = address + chip->V[0];
        break;

    default:
        return false; 
    }
    return true;
}