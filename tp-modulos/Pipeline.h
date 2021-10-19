#ifndef PIPELINE_H
#define PIPELINE_H
#include "machine.h"

Instruction load(Instruction* compiledInstructions, int PC);
void execute(Instruction* inst, int opcode, double* RAM);
int decode(Instruction inst);

#endif