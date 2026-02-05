#include "OpCode.h"
#include <stdio.h>
#include <string.h>

/*
    CHIP-8 OpCodes Logic - SUPER-CHIP specification from 1991
*/



/*
* movePc - sets the value of PC occurding to the opcode
* params : (boolean: debugMode , Chip8* chip)
*/
bool movePC(bool debugMode,Chip8* chip)
{
    bit16 opcode = chip->opcode;
    bit8 op = (opcode & 0xF000) >> 12;
    bit16 address = (opcode & 0x0FFF);


    //0x00EE - Returns from a subroutine
    if (opcode == 0x00EE) {
        chip->PC = chip->stack[--chip->SP];
        if (debugMode) { printf("opcode = Return from stack\n"); }
        return true;
    }
    
    switch (op) {

        // 0x1NNN - Jumps to address NNN
        case 0x1: 
            chip->PC = address;
            if (debugMode) { printf("opcode = Jump to 0x%03X\n", address); }
            return true;

        //0x2NNN - Calls subroutine at NNN
        case 0x2:  
            chip->stack[chip->SP] = chip->PC;
            chip->SP++;
            chip->PC = address;
            if (debugMode) { printf("opcode = Call 0x%03X\n", address); }
            return true;
        
        //0xBNN - Jumps to the address NNN plus V0
        case 0xB: 
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

        //0x3XNN - Skips the next instruction if VX equals NN (usually the next instruction is a jump to skip a code block)
        case 0x3: 
            if (chip->V[x] == nn) { chip->PC += 2; }
            if (debugMode) { printf("opcode = if VX == NN\n"); }
            return true;

        //0x4XNN - Skips the next instruction if VX does not equal NN (usually the next instruction is a jump to skip a code block)
        case 0x4: 
            if (chip->V[x] != nn) { chip->PC += 2; }
            if (debugMode) { printf("opcode = if VX != NN\n"); }
            return true;

        //0x5XY0 -  Skips the next instruction if VX equals VY (usually the next instruction is a jump to skip a code block)
        case 0x5: 
            if (chip->V[x] == chip->V[y]) { chip->PC += 2; }
            if (debugMode) { printf("opcode = if VX == VY\n"); }
            return true;

        //0x9XY0 - Skips the next instruction if VX does not equal VY. (Usually the next instruction is a jump to skip a code block)
        case 0x9: 
            if (chip->V[x] != chip->V[y]) { chip->PC += 2; }
            if (debugMode) { printf("opcode = if VX != VY\n"); }
            return true;

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

    //0x6XNN - Sets VX to NN
    case 0x6: 
        chip->V[x] = nn;
        if (debugMode) printf("opcode : V[%X] = %02X\n", x, nn);
        return true;
    
    //0x7XNN - Adds NN to VX (carry flag is not changed).
    case 0x7: 
        chip->V[x] += nn;
        if (debugMode) printf("opcode : V[%X] += %02X\n", x, nn);
        return true;

    case 0x8:
        switch (opR) {

        // 8XY0: - Sets VX to the value of VY
        case 0x0: 
            chip->V[x] = chip->V[y];
            if (debugMode) printf("opcode : V[%X] = V[%X]\n", x, y);
            return true;

        //0x8XY4 - Adds VY to VX. VF is set to 1 when there's an overflow, and to 0 when there is not
        case 0x4: 
        {
            bit16 sum = (bit16)chip->V[x] + (bit16)chip->V[y];
            chip->V[0xF] = (sum > 255) ? 1 : 0; // Carry
            chip->V[x] = (bit8)(sum & 0xFF);
            if (debugMode) printf("opcode : V[%X] += V[%X] (VF=%d)\n", x, y, chip->V[0xF]);
            return true;
        }

        //0x8XY5 - VY is subtracted from VX. VF is set to 0 when there's an underflow, and 1 when there is not. (i.e. VF set to 1 if VX >= VY and 0 if not)
        case 0x5: 
        {
            bit8 vX_old = chip->V[x];
            chip->V[0xF] = (chip->V[x] >= chip->V[y]) ? 1 : 0; // NOT Borrow 
            chip->V[x] -= chip->V[y];
            if (debugMode) printf("opcode : V[%X] -= V[%X] (VF=%d)\n", x, y, chip->V[0xF]);
            return true;
        }

        //0x8XY7 - Sets VX to VY minus VX. VF is set to 0 when there's an underflow, and 1 when there is not. (i.e. VF set to 1 if VY >= VX).
        case 0x7:
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

    //0x8XY1 - Sets VX to VX or VY. (bitwise OR operation)
    case 0x1:
        chip->V[x] |= chip->V[y];
        if (debugMode) { printf("Vx = Vx OR Vy\n"); }
        return true;

    //8XY2 - Sets VX to VX and VY. (bitwise AND operation)
    case 0x2:
        chip->V[x] &= chip->V[y];
        if (debugMode) { printf("Vx = Vx AND Vy\n"); }
        return true;

    // --- The logical opcodes 8XY3, 8XY6, 8XY7 and 8XYE were not documented in the original CHIP-8 specification, as all the 8000 opcodes were dispatched to instructions in the 1802's ALU, and not located in the interpreter itself; these four additional opcodes were therefore presumably unintentional functionality.---
    //0x8XY3 - Sets VX to (VX xor VY)
    case 0x3:
        chip->V[x] ^= chip->V[y];
        if (debugMode) { printf("Vx = Vx XOR Vy\n"); }
        return true;
    
    //0x8XY6 - Shifts VX to the right by 1, then stores the least significant bit of VX prior to the shift into VF
    case 0x6: 
        //CHIP-48 and SCHIP implementation
        chip->V[0xF] = chip->V[x] & 0x1;    
        chip->V[x] >>= 1;
        //original implemetaion such as COSMAC VIP :  chip->V[0xF] = chip->V[y] & 0x1; chip->V[x] = chip->V[y] >> 1;
        if (debugMode) { printf("VF = VX AND  0x1\n"); }
        return true;

    //0x8XYE - Shifts VX to the left by 1, then sets VF to 1 if the most significant bit of VX prior to that shift was set, or to 0 if it was unset
    case 0xE: 
        //CHIP-48 and SCHIP implementation
        chip->V[0xF] = (chip->V[x] & 0x80) >> 7; //sets VF to 1 or 0 dependes on the MSB of VX
        chip->V[x] <<= 1;
        //original implemetaion such as COSMAC VIP : chip->V[0xF] = (chip->V[y] & 0x80) >> 7; chip->V[x] = chip->V[y] <<  1;
        if (debugMode) { printf("VF = MSB of VX, VX <<= 1\n"); }
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
    bit8 lowestNibble;
    switch (op) {

    //ANNN - Sets I to the address NNN
    case 0xA:
        chip->I = nnn;
        if (debugMode) { printf("opcode : I = NNN\n"); }
        return true;
    
    case 0xF:
        switch (indexOp) {
        //0xFX1E - Adds VX to I. VF is not affected.
        case 0x1E:
            chip->I += chip->V[x];
            if (debugMode) { printf("opcode : I += VX\n"); }
            return true;

        //0xFX29 - Sets I to the location of the sprite for the character in VX(only consider the lowest nibble). Characters 0-F (in hexadecimal) are represented by a 4x5 font
        case 0x29:
            lowestNibble = chip->V[x] & 0x0F;
            chip->I = FONT_ADDRESS_START + (lowestNibble * SIZE_OF_FONT_INSTANCE);
            if (debugMode) { printf("opcode : I = FONT_ADDRESS[VX]  \n"); }
            return true;

        //0xFX33 - Stores the binary-coded decimal representation of VX, with the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.
        case 0x33:
            chip->RAM[chip->I] = chip->V[x] / 100;         
            chip->RAM[chip->I + 1] = (chip->V[x] / 10) % 10;   
            chip->RAM[chip->I + 2] = chip->V[x] % 10;
            if (debugMode) { printf("opcode : write to Ram in address I to I+2 the val of VX \n"); }
            return true;

        //0xFX55 - Stores from V0 to VX (including VX) in memory, starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified
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

        //0xF65 - Fills from V0 to VX (including VX) with values from memory, starting at address I. The offset from I is increased by 1 for each value read, but I itself is left unmodified.
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
    bit8 lowestNibble;

    switch (opStart) {

    //0x00E0 - Clears the screen 
    case 0x0:
        memset(chip->screen, 0, sizeof(chip->screen));
        if (debugMode) { printf("opcode = Clear Screen\n"); }
        return true;

    //DXYN - draw sprites. see explenation above function drawSprite
    case 0xD:
        drawSprite(chip, chip->V[x], chip->V[y], n);
        if (debugMode) { printf("opcode : draw sprite in (Vx,Vy) with height N \n"); }
        return true;
         
    
    case 0xE:
        switch (opEnd)
        {
            //0xEX9E - Skips the next instruction if the key stored in VX (only consider the lowest nibble) is pressed (usually the next instruction is a jump to skip a code block)
            case 0x9E:
                lowestNibble = chip->V[x] & 0x0F;
                if (chip->keys[lowestNibble] == 1) { chip->PC += 2;}
                if (debugMode) { printf("opcode : check if key[vX] is pressed if yes then skip next instruction \n"); }
                return true;

            //0xEXA1 - Skips the next instruction if the key stored in VX(only consider the lowest nibble) is not pressed (usually the next instruction is a jump to skip a code block)
            case 0xA1:
                lowestNibble = chip->V[x] & 0x0F;
                if (chip->keys[lowestNibble] == 0) { chip->PC += 2; }
                if (debugMode) { printf("opcode : check if key[vX] is not pressed if yes then skip next instruction \n"); }
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
            //0xFX07  - Sets VX to the value of the delay timer
            case 0x07:
                chip->V[x] = chip->delay_timer;
                return true;

            //0xFX15 - Sets the delay timer to VX
            case 0x15:
                chip->delay_timer = chip->V[x];
                return true;

            //0xFX18 - Sets the sound timer to VX
            case 0x18:
                chip->sound_timer = chip->V[x];
                return true;

            default:
                return false;
        }
       
    //0xCXNN - Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN
    case 0xC:
        chip->V[x] = (rand() % 256) & nn;
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

    //static cases
    switch (chip->opcode) {
    case 0x00E0:
        return IOandP(debugMode, chip);
    case 0x00EE:
        return movePC(debugMode, chip);
    }


    //dynamic cases
    switch (startWith) {
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