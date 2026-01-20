#ifndef OPCODE_H
#define OPCODE_H

#include "HardWare.h" 


bool fetchAndPrcocessOpCode(bool debugMode, Chip8 * chip);

bool movePC(bool debugMode, Chip8* chip);

bool cond(bool debugMode, Chip8* chip);

bool mathAndAssign(bool debugMode, Chip8* chip);

bool bitWiseOp(bool debugMode, Chip8* chip);

bool memoryAndIndexing(bool debugMode, Chip8* chip);

bool IOandP(bool debugMode, Chip8* chip);

#endif // OPCODE_H
