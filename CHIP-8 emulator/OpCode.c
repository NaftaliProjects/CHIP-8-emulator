#include "OpCode.h"
#include <stdio.h>

/*
* movePc - sets the value of PC occurding to the opcode
* params : (boolean: debugMode , Chip8* chip)
*/
bool movePC(bool debugMode,Chip8* chip)
{
    bit16 opcode = chip->opcode;
    bit8 op = (opcode & 0xF000) >> 12;
    bit16 address = (opcode & 0x0FFF);

    if (op == 0x0) {
        if (opcode == 0x00EE) {
            chip->PC = chip->stack[--chip->SP];
            if (debugMode) { printf("opcode = Return from stack\n"); }
            return true;
        }
        if (opcode == 0x00E0) {
            if (debugMode) { printf("opcode = Clear Screen\n"); }
            return true;
        }
        return true;
    }
    
    
    switch (op) {
        case 0x1: // Jump 1NNN
            chip->PC = address;
            if (debugMode) { printf("opcode = Jump to 0x%03X\n", address); }
            return true;

        case 0x2: // Call 2NNN
            chip->stack[chip->SP] = chip->PC;
            chip->SP++;
            chip->PC = address;
            if (debugMode) { printf("opcode = Call 0x%03X\n", address); }
            return true;

        case 0xB: // Jump + V0
            chip->PC = address + chip->V[0];
            if (debugMode) { printf("opcode : Jump + V0 =  0x%03X \n", chip->PC); }
            return true;

        default:
            if (debugMode) { printf("opcode isnt a move\n"); }
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
    bit8 nn = (opcode & 0x00FF);
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



bool bitWiseOp(bit16 opcode, Chip8* chip)
{
    //no need to process F000 becuase it is always 8
    bit8 op = (opcode & 0x000F);
    bit8 x = (opcode & 0x0F00) >> 8;
    bit8 y = (opcode & 0x00F0) >> 4;

    printf("op 0x00F = 0x%04X  d\n", op);
    

    switch (op) {
    case 0x1:
        chip->V[x] |= chip->V[y];
        printf("Vx = Vx OR Vy\n");
        return true;
    case 0x2:
        chip->V[x] &= chip->V[y];
        printf("Vx = Vx AND Vy\n");
        return true;
    case 0x3:
        chip->V[x] ^= chip->V[y];
        printf("Vx = Vx XOR Vy\n");
        return true;
    case 0x6: 
        chip->V[0xF] = (chip->V[x] & 0x1);
        chip->V[x] >>= 1;
        return true;
    case 0xE: 
        chip->V[0xF] = (chip->V[x] & 0x80) >> 7;
        chip->V[x] <<= 1;
        return true;


    default:
        printf("opcode isnt bitwise \n");
        return false;
    }
}



bool memoryAndIndexing(bit16 opcode, Chip8* chip)
{
    bit8 op = (opcode & 0xF000) >> 12;
    bit8 nnn = (opcode & 0x0FFF);
    bit8 x = (opcode & 0x0F00) >> 8;
    bit8 indexOp = (opcode & 0x00FF);

    switch (op) {
    case 0xA:
        chip->I = nnn;
        return true;

    case 0xF:
        switch (indexOp) {
        case 0x1E:
            chip->I += chip->V[x];
            return true;

        case 0x29:
            chip->I = chip->V[x] * SIZE_OF_FONT_INSTANCE;
            return true;

        case 0x33:
            chip->RAM[chip->I] = chip->V[x] / 100;         
            chip->RAM[chip->I + 1] = (chip->V[x] / 10) % 10;   
            chip->RAM[chip->I + 2] = chip->V[x] % 10;
            return true;

        case 0x55:
            if (chip->I + x >= 4096) {
                printf("Error: Reg Dump out of RAM bounds!\n");
                return false;
            }
            for (int i = 0; i <= x; i++)
            {
                chip->RAM[chip->I + i] = chip->V[i];
            }
            return true;

        case 0x65:
            if (chip->I + x >= 4096) {
                printf("Error: Reg Load out of RAM bounds!\n");
                return false;
            }
            for (int i = 0; i <= x; i++)
            {
                chip->V[i] = chip->RAM[chip->I + i];
            }
            return true;

        default:
            return false;
        }

    default:
        printf("opcode isnt memoryAndIndexing \n");
        return false;
    }
}



bool IOandP(bit16 opcode, Chip8* chip)
{
    bit8 opStart = (opcode & 0xF000) >> 12;
    bit8 opEnd = (opcode & 0x00FF);
    bit8 x = (opcode & 0x0F00) >> 8;
    bit8 y = (opcode & 0x00F0) >> 4;
    bit8 n = (opcode & 0x000F);
    bit8 nn = opEnd;
    
    bit8 indexOp = (opcode & 0x00FF);


    switch (opStart) {
    case 0x00E0:
        //clear screen
        return true;

    case 0xD:
        //draw(chip->V[x], chip->V[y], n);
        return true;
         
    case 0xE:
        switch (opEnd)
        {
            case 0x9E:
                //if (chip->keyPressed == chip->V[x])
                    //skipNext(&chip);
                return true;

            case 0xA1:
                //if (chip->keyPressed != chip->V[x])
                    //skipNext(&chip);
                return true;

            default:
                return false;
        }
        
       
    case 0xF:
        switch (opEnd)
        {
            case 0x0A:
                //chip->V[x] = getKey(&chip);
                return true;

            case 0x07:
                //chip->V[x] = getDelay(&chip->delayTimer)
                return true;

            case 0x15:
                //chip->delayTimer = chip->V[x];
                return true;

            case 0x18:
                //chip->sound_timer = chip->V[x];
                return true;

            default:
                return false;
        }
       
    case 0xC:
        //chip->V[x] = rand() & nn;
        return true;

    default:
        return false;
    }
}



bool fetchAndPrcocessOpCode(Chip8* chip)
{
    if (chip->PC >= MIN_OTHER_RESERVED_ADDRESS - 1) {
        return false;
    }

    // Fetch
    chip->opcode = (chip->RAM[chip->PC] << 8) | chip->RAM[chip->PC + 1];
    chip->PC += 2;

    // Extract first nibble correctly (Shift Right >>)
    bit8 startWith = (chip->opcode & 0xF000) >> 12;
    bit8 endWith = (chip->opcode & 0x000F);

    switch (startWith) {

        case 0x0:
        case 0x1:
        case 0x2:
        case 0xB:
            return movePC(chip->opcode, chip); 

        case 0x3:
        case 0x4:
        case 0x5:
        case 0x9:
            return cond(chip->opcode, chip);

        case 0x6:
        case 0x7:
            return mathAndAssign(chip->opcode, chip);

        case 0x8:
            if (endWith == 0x0 || endWith == 0x4 || endWith == 0x5 || endWith == 0x7)
                return mathAndAssign(chip->opcode, chip);

            else if (endWith == 0x1 || endWith == 0x2 || endWith == 0x3 || endWith == 0x6 || endWith == 0xE)
                return bitWiseOp(chip->opcode, chip);

        case 0xA:
        case 0xF:

            return memoryAndIndexing(chip->opcode, chip);

        case 0xD:
        case 0xE:
        case 0xC:
            return IOandP(chip->opcode, chip);

        default: 
            return false;
    }
}