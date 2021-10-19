#ifndef MACHINE_H
#define MACHINE_H

typedef struct
{
    int opCode;
    double addressOne;
    double addressTwo;
    double addressThree;
} Instruction;

double *createRAM(double *RAM);
void machine(Instruction *instructions, double *RAM);
void interpretedMachine(Instruction *inst, double *RAM);
Instruction *toCompile(Instruction *instructions);
Instruction load(Instruction *compiledInstructions, int PC);

#endif