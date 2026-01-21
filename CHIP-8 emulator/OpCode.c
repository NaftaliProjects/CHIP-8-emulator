#include "OpCode.h"
#include <stdio.h>
#include <string.h>
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





/*
* cond - handle condtion branch opcodes
* params : (boolean: debugMode , Chip8* chip)
*/
bool cond(bool debugMode, Chip8* chip)
{
    bit16 opcode = chip->opcode;

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
        if (debugMode) { printf("opcode = if VX == NN\n"); }
        return true;
        break;

    case 0x4: 
        if (chip->V[x] != nn)
        {
            chip->PC += 2;
        }
        if (debugMode) { printf("opcode = if VX != NN\n"); }
        return true;
        break;

    case 0x5: 
        if (chip->V[x] == chip->V[y])
        {
            chip->PC += 2;
        }
        if (debugMode) { printf("opcode = if VX == VY\n"); }
        return true;
        break;

    case 0x9: 
        if (chip->V[x] != chip->V[y])
        {
            chip->PC += 2;
        }
        if (debugMode) { printf("opcode = if VX != VY\n"); }
        return true;
        break;
    default:
        if (debugMode) { printf("opcode isnt a cond\n"); }
        return false;
    }
}


/*
* mathAndAssign - Assign VX with math operations
* params : (boolean: debugMode , Chip8* chip)
*/
bool mathAndAssign(bool debugMode, Chip8* chip) {
    bit16 opcode = chip->opcode;

    bit8 op = (opcode & 0xF000) >> 12;
    bit8 x = (opcode & 0x0F00) >> 8;
    bit8 y = (opcode & 0x00F0) >> 4;
    bit8 nn = (opcode & 0x00FF);
    bit8 opR = (opcode & 0x000F);

    switch (op) {
    case 0x6: // 6XNN: VX = NN
        chip->V[x] = nn;
        if (debugMode) printf("opcode : V[%X] = %02X\n", x, nn);
        return true;

    case 0x7: // 7XNN: VX += NN (Carry flag NOT affected)
        chip->V[x] += nn;
        if (debugMode) printf("opcode : V[%X] += %02X\n", x, nn);
        return true;

    case 0x8:
        switch (opR) {
        case 0x0: // 8XY0: VX = VY
            chip->V[x] = chip->V[y];
            if (debugMode) printf("opcode : V[%X] = V[%X]\n", x, y);
            return true;

        case 0x4: // 8XY4: VX += VY (Affects Carry Flag)
        {
            bit16 sum = (bit16)chip->V[x] + (bit16)chip->V[y];
            chip->V[0xF] = (sum > 255) ? 1 : 0; // Carry
            chip->V[x] = (bit8)(sum & 0xFF);
            if (debugMode) printf("opcode : V[%X] += V[%X] (VF=%d)\n", x, y, chip->V[0xF]);
            return true;
        }

        case 0x5: // 8XY5: VX -= VY (Affects Borrow Flag)
        {
            bit8 vX_old = chip->V[x];
            chip->V[0xF] = (chip->V[x] >= chip->V[y]) ? 1 : 0; // NOT Borrow
            chip->V[x] -= chip->V[y];
            if (debugMode) printf("opcode : V[%X] -= V[%X] (VF=%d)\n", x, y, chip->V[0xF]);
            return true;
        }

        case 0x7: // 8XY7: VX = VY - VX (Affects Borrow Flag)
        {
            chip->V[0xF] = (chip->V[y] >= chip->V[x]) ? 1 : 0; // NOT Borrow
            chip->V[x] = chip->V[y] - chip->V[x];
            if (debugMode) printf("opcode : V[%X] = V[%X] - V[%X] (VF=%d)\n", x, y, x, chip->V[0xF]);
            return true;
        }

        default:
            return false;
        }

    default:
        return false;
    }
}


/*
* bitWiseOp - Assign VX with bitwise operator 
* params : (boolean: debugMode , Chip8* chip)
*/
bool bitWiseOp(bool debugMode, Chip8* chip)
{
    bit16 opcode = chip->opcode;
    //no need to process F000 becuase it is always 8
    bit8 op = (opcode & 0x000F);
    bit8 x = (opcode & 0x0F00) >> 8;
    bit8 y = (opcode & 0x00F0) >> 4;

    

    switch (op) {
    case 0x1:
        chip->V[x] |= chip->V[y];
        if (debugMode) { printf("Vx = Vx OR Vy\n"); }
        return true;
    case 0x2:
        chip->V[x] &= chip->V[y];
        if (debugMode) { printf("Vx = Vx AND Vy\n"); }
        return true;
    case 0x3:
        chip->V[x] ^= chip->V[y];
        if (debugMode) { printf("Vx = Vx XOR Vy\n"); }
        return true;
    case 0x6: 
        chip->V[0xF] = chip->V[x] & 0x1;
        chip->V[x] >>= 1;
        if (debugMode) { printf("VF = VX AND  0x1\n"); }
        return true;
    case 0xE: 
        chip->V[0xF] = (chip->V[x] & 0x80) >> 7;
        chip->V[x] <<= 1;
        if (debugMode) { printf("VF = Vx AND 0x80\n"); }
        return true;


    default:
        if (debugMode) { printf("opcode isnt bitwise \n"); }
        return false;
    }
}



/*
* memoryAndIndexing - Assign VX with bitwise operator
* params : (boolean: debugMode , Chip8* chip)
*/
bool memoryAndIndexing(bool debugMode, Chip8* chip)
{
    bit16 opcode = chip->opcode;
    bit8 op = (opcode & 0xF000) >> 12;
    bit16 nnn = (opcode & 0x0FFF);
    bit8 x = (opcode & 0x0F00) >> 8;
    bit8 indexOp = (opcode & 0x00FF);

    switch (op) {
    case 0xA:
        chip->I = nnn;
        if (debugMode) { printf("opcode : I = NNN\n"); }
        return true;

    case 0xF:
        switch (indexOp) {
        case 0x1E:
            chip->I += chip->V[x];
            if (debugMode) { printf("opcode : I += VX\n"); }
            return true;

        case 0x29:
            chip->I = FONT_ADDRESS_START + (chip->V[x] * SIZE_OF_FONT_INSTANCE); 
            return true;

        case 0x33:
            chip->RAM[chip->I] = chip->V[x] / 100;         
            chip->RAM[chip->I + 1] = (chip->V[x] / 10) % 10;   
            chip->RAM[chip->I + 2] = chip->V[x] % 10;
            if (debugMode) { printf("opcode : write to Ram in address I to I+2 the val of VX \n"); }
            return true;

        case 0x55:
            if (chip->I + x >= 4096) {
                if (debugMode) { printf("Error: Reg Load out of RAM bounds!\n"); }
                return false;
            }
            for (int i = 0; i <= x; i++)
            {
                chip->RAM[chip->I + i] = chip->V[i];
            }
            if (debugMode) { printf("opcode : RAM[I+i] = Vi\n"); }
            return true;

        case 0x65:
            if (chip->I + x >= 4096) {
                if (debugMode) { printf("Error: Reg Load out of RAM bounds!\n"); }
                return false;
            }
            for (int i = 0; i <= x; i++)
            {
                chip->V[i] = chip->RAM[chip->I + i];
            }
            if (debugMode) { printf("opcode : Vi = RAM[I+i]\n"); }
            return true;

        default:
            return false;
        }

    default:
        if (debugMode) { printf("opcode isnt memoryAndIndexing \n"); }
        return false;
    }
}


/*
* IOandP - handle IO control and Peripherals
* params : (boolean: debugMode , Chip8* chip)
*/
bool IOandP(bool debugMode, Chip8* chip)
{
    bit16 opcode = chip->opcode;
    bit8 opStart = (opcode & 0xF000) >> 12;
    bit8 opEnd = (opcode & 0x00FF);
    bit8 x = (opcode & 0x0F00) >> 8;
    bit8 y = (opcode & 0x00F0) >> 4;
    bit8 n = (opcode & 0x000F);
    bit8 nn = opEnd;
    bit8 nnn = (opcode & 0x0FFF);
    
    bit8 indexOp = (opcode & 0x00FF);


    switch (opStart) {
    case 0x0:
        memset(chip->screen, 0, sizeof(chip->screen));
        if (debugMode) { printf("opcode = Clear Screen\n"); }
        return true;

    case 0xD:
        drawSprite(chip, chip->V[x], chip->V[y], n);
        if (debugMode) { printf("opcode : draw sprite in (Vx,Vy) with height N \n"); }
        return true;
         
    case 0xE:
        switch (opEnd)
        {
            case 0x9E:
                if (chip->keys[chip->V[x]] == 1) {
                    chip->PC += 2;
                }
                return true;


            case 0xA1:
                if (chip->keys[chip->V[x]] == 0) {
                    chip->PC += 2;
                }
                return true;

            default:
                return false;
        }
        
       
    case 0xF:
        switch (opEnd)
        {
        case 0x0A:
        {
            bool keyPressed = false;
            for (int i = 0; i < 16; i++) {
                if (chip->keys[i] == 1) {
                    chip->V[x] = i;
                    keyPressed = true;
                    break;
                }
            }

            if (!keyPressed) {
                chip->PC -= 2;
            }
            return true;
        }

            case 0x07:
                chip->V[x] = chip->delay_timer;
                return true;

            case 0x15:
                chip->delay_timer = chip->V[x];
                return true;

            case 0x18:
                chip->sound_timer = chip->V[x];
                return true;

            default:
                return false;
        }
       
    case 0xC:
        chip->V[x] = rand() & nn;
        return true;

    default:
        return false;
    }
}



bool fetchAndPrcocessOpCode(bool debugMode,Chip8* chip)
{
    if (chip->PC >= MIN_OTHER_RESERVED_ADDRESS - 1) {
        return false;
    }

    // Fetch
    chip->opcode = (chip->RAM[chip->PC] << 8) | chip->RAM[chip->PC + 1];
    chip->PC += 2;

    if (debugMode) { printf("\nprocessing opcode : 0x%04X \n", chip->opcode); }

    // Extract first nibble correctly (Shift Right >>)
    bit8 startWith = (chip->opcode & 0xF000) >> 12;
    bit8 endWith = (chip->opcode & 0x000F);

    switch (startWith) {

        case 0x0:
            if (chip->opcode == 0x00E0)
                return IOandP(debugMode, chip);
            else if (chip->opcode == 0x00EE)
                return movePC(debugMode, chip);
            else
                return false;

        case 0x1:
        case 0x2:
        case 0xB:
            return movePC(debugMode, chip); 

        case 0x3:
        case 0x4:
        case 0x5:
        case 0x9:
            return cond(debugMode, chip);

        case 0x6:
        case 0x7:
            return mathAndAssign(debugMode, chip);

        case 0x8:
            if (endWith == 0x0 || endWith == 0x4 || endWith == 0x5 || endWith == 0x7)
                return mathAndAssign(debugMode, chip);

            else if (endWith == 0x1 || endWith == 0x2 || endWith == 0x3 || endWith == 0x6 || endWith == 0xE)
                return bitWiseOp(debugMode, chip);

        case 0xA:
            return memoryAndIndexing(debugMode, chip);

        case 0xF:
        {
            bit8 opEnd = chip->opcode & 0x00FF;

            // Timers / Keyboard / Sound
            if (opEnd == 0x07 || opEnd == 0x0A || opEnd == 0x15 || opEnd == 0x18)
                return IOandP(debugMode, chip);

            // Memory / Index instructions
            else if (opEnd == 0x1E || opEnd == 0x29 || opEnd == 0x33 || opEnd == 0x55 || opEnd == 0x65)
                return memoryAndIndexing(debugMode, chip);

            else {
                if (debugMode) { printf("Unknown F opcode: %04X\n", chip->opcode); }
                return false;
            }
        }
            

        case 0xD:
        case 0xE:
        case 0xC:
            return IOandP(debugMode, chip);

        default: 
            printf("Unknown opcode or Unknown error");
            return false;
    }
}