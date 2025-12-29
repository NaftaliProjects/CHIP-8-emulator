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
        printf("opcode = Jump to 0x%03X\n", address);
        break;

    case 0x2: // Call 2NNN
        chip->stack[chip->SP] = chip->PC;
        chip->SP++;
        chip->PC = address;
        printf("opcode = Call 0x%03X\n", address);
        break;

    case 0xB: // Jump + V0
        chip->PC = address + chip->V[0];
        printf("opcode : Jump + V0 =  0x%03X \n",chip->PC);
        break;

    default:
        printf("opcode isnt a move\n");
        return false;
    }
   
}


bool cond(bit16 opcode, Chip8* chip)
{
    bit8 op = (opcode & 0xF000) >> 12;
    bit16 nn = (opcode & 0x00FF);
    bit8 x = (opcode & 0x0F00) >> 8;
    bit8 y = (opcode & 0x00F0) >> 4;
    switch (op) {
    case 0x3: 
        if (chip->V[x] == nn)
        {
            chip->PC+=2;
        }
        printf("opcode = if VX == NN\n");
        return true;
        break;

    case 0x4: 
        if (chip->V[x] != nn)
        {
            chip->PC += 2;
        }
        printf("opcode = if VX == NN\n");
        return true;
        break;

    case 0x5: 
        if (chip->V[x] == chip->V[y])
        {
            chip->PC += 2;
        }
        printf("opcode = if VX == NN\n");
        return true;
        break;

    case 0x9: 
        if (chip->V[x] != chip->V[y])
        {
            chip->PC += 2;
        }
        printf("opcode = if VX == NN\n");
        return true;
        break;
    default:
        printf("opcode isnt a cond\n");
        return false;
    }
}


bool mathAndAssign(bit16 opcode, Chip8* chip)
{
    bit8 op = (opcode & 0xF000) >> 12;
    bit8 x = (opcode & 0x0F00) >> 8;
    bit8 y = (opcode & 0x00F0) >> 4;
    bit16 nn = (opcode & 0x00FF);
    bit8 opR = (opcode & 0x000F);
    if (op == 0x8)
    {
        op = op + opR;
    }

    switch (op) {
    case 0x6:
        chip->V[x] = nn;
        printf("opcode = Vx = NN\n");
        return true;
        break;
    case 0x7:
        chip->V[x] += nn;
        printf("opcode = Vx += NN\n");
        return true;
        break;
    case (0x8+ 0x0):
        chip->V[x] = chip->V[y];
        printf("opcode = Vx = Vy\n");
        return true;
        break;
    case (0x8 + 0x4):
        chip->V[x] += chip->V[y];
        printf("opcode = Vx += Vy\n");
        return true;
        break;
    case (0x8 + 0x5):
        chip->V[x] -= chip->V[y];
        printf("opcode = Vx -= Vy\n");
        return true;
        break;
    case (0x8 + 0x7):
        chip->V[x] = chip->V[y] - chip->V[x];
        printf("opcode = Vx = Vy - Vx\n");
        return true;
        break;

    
    default:
        printf("opcode isnt math or assing\n");
        return false;
    }
}