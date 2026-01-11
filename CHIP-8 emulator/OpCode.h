#ifndef OPCODE_H
#define OPCODE_H

#include "HardWare.h" 


bool fetchAndPrcocessOpCode(Chip8* chip);

bool movePC(bit16 opcode, Chip8* chip);

bool cond(bit16 opcode, Chip8* chip);

bool mathAndAssign(bit16 opcode, Chip8* chip);

bool bitWiseOp(bit16 opcode, Chip8* chip);

bool memoryAndIndexing(bit16 opcode, Chip8* chip);

bool IOandP(bit16 opcode, Chip8* chip);

#endif // OPCODE_H
