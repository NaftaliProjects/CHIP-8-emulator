#pragma once
#include "HardWare.h" 
bool movePC(bit16 opcode,Chip8* chip);
bool cond(bit16 opcode, Chip8* chip);
bool mathAndAssign(bit16 opcode, Chip8* chip);

