#include "Pipeline.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_VALUE 100000
#define null -2

#define ANSI_RESET "\x1b[0m"  // desativa os efeitos anteriores
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define RED(string) ANSI_COLOR_RED string ANSI_RESET
#define GREEN(string) ANSI_COLOR_GREEN string ANSI_RESET

Instruction load(Instruction* compiledInstructions, int PC) {
    if (PC >= 0) {
        return compiledInstructions[PC];
    } else {
        compiledInstructions[PC].opCode = null;
        return compiledInstructions[PC];
    }
}

int decode(Instruction inst) {
    if (inst.opCode != null) {
        return inst.opCode;
    } else {
        return -1;
    }
}

void execute(Instruction* inst, int opcode, double* RAM) {
    switch (opcode) {
            // SOMAR
        case 0: {
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
        case 1: {
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
        case 2: {
            double content = inst->addressOne;
            int add = (int)inst->addressTwo;
            RAM[add] = content;
            break;
        }
            // trazer da RAM
        case 3: {
            inst->addressOne = RAM[(int)inst->addressTwo];
            break;
        }
    }
}
