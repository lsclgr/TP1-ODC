#include "machine.h"
#include "Pipeline.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_VALUE 100000

#define ANSI_RESET "\x1b[0m" // desativa os efeitos anteriores
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define RED(string) ANSI_COLOR_RED string ANSI_RESET
#define GREEN(string) ANSI_COLOR_GREEN string ANSI_RESET

double *createRAM(double *RAM)
{
    RAM = (double *)malloc(1000 * sizeof(double));

    for (int i = 0; i < 1000; i++)
    {
        RAM[i] = rand() % 100;
    }
    return RAM;
}

Instruction *toCompile(Instruction *instructions)
{
    // aqui teria o q o GCC faz, que demorou décadas para ser feito
    // eficientemente
    return instructions;
}

void machine(Instruction *instructions, double *RAM)
{

    Instruction *compiledInstructions = malloc(100 * sizeof(Instruction));
    compiledInstructions = toCompile(instructions);

    int PC = 0, opcode = MAX_VALUE;

    while (opcode != -1)
    {
        Instruction inst = load(compiledInstructions, PC);
        opcode = decode(inst);
        execute(&inst, opcode, RAM);
        PC++;
    }
}

void interpretedMachine(Instruction *inst, double *RAM)
{
    int opcode = inst->opCode;
    switch (opcode)
    {
        // SOMAR
    case 0:
    {
        int add1 = (int)inst->addressOne;
        int add2 = (int)inst->addressTwo;
        // buscar na RAM
        double contentRAM1 = RAM[add1];
        double contentRAM2 = RAM[add2];
        double sum = contentRAM1 + contentRAM2;
        // salvando resultado na RAM
        int add3 = (int)inst->addressThree;
        RAM[add3] = sum;

        printf(GREEN("somando %.1lf com %.1lf e gerando %.1lf!") "\n",
               contentRAM1, contentRAM2, sum);
        break;
    }
        // SUBTRAIR
    case 1:
    {
        int add1 = (int)inst->addressOne;
        int add2 = (int)inst->addressTwo;
        // buscar na RAM
        double contentRAM1 = RAM[add1];
        double contentRAM2 = RAM[add2];
        double sub = contentRAM1 - contentRAM2;
        // salvando resultado na RAM
        int add3 = (int)inst->addressThree;
        RAM[add3] = sub;

        printf(RED("subtraindo %.1lf com %.1lf e gerando %.1lf!") "\n",
               contentRAM1, contentRAM2, sub);
        break;
    }
        // levar para RAM
    case 2:
    {
        double content = inst->addressOne;
        int add = (int)inst->addressTwo;
        RAM[add] = content;
        break;
    }
        // trazer da RAM
    case 3:
    {
        inst->addressOne = RAM[(int)inst->addressTwo];
        break;
    }
    }
}