#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_VALUE 100000

typedef struct {
    int opCode;
    int addressOne;
    int addressTwo;
    int addressThree;
} Instruction;

int *createRAM(int *RAM);
void createRandomInstructions(int *RAM);
void machine(Instruction *instructions, int *RAM);
void interpretedMachine(Instruction inst, int *RAM);
Instruction *toCompile(Instruction *instructions);
void createProgramMultiply(int multiplicand, int multiplier, int *RAM);
void createProgramDivide(int dividend, int divisor);

int main() {
    srand(time(NULL));
    int *RAM = NULL;
    RAM = createRAM(RAM);

    createRAM(RAM);
    createRandomInstructions(RAM);
    createProgramMultiply(400, 3, RAM);

    return 0;
}

int *createRAM(int *RAM) {
    RAM = (int *)malloc(1000 * sizeof(int));

    for (int i = 0; i < 1000; i++) {
        RAM[i] = rand() % 100;
    }
    return RAM;
}

void createRandomInstructions(int *RAM) {
    // 01|22|13|45 => isto é uma instrução
    // 00|33|12|01 => isto é outra instrução

    // 0 => opcode => somar
    // 1 => opcode => subtrair
    //-1 => halt -> parar
    Instruction *instructions = malloc(100 * sizeof(Instruction));
    Instruction inst;

    for (int i = 0; i < 99; i++) {
        inst.opCode = rand() % 2;
        inst.addressOne = rand() % 1000;
        inst.addressTwo = rand() % 1000;
        inst.addressThree = rand() % 1000;
        instructions[i] = inst;
    }
    inst.opCode = -1;
    inst.addressOne = -1;
    inst.addressTwo = -1;
    inst.addressThree = -1;
    instructions[99] = inst;

    machine(instructions, RAM);
}
Instruction *toCompile(Instruction *instructions) {
    // aqui teria o q o GCC faz, que demorou décadas para ser feito
    // eficientemente
    return instructions;
}

void machine(Instruction *instructions, int *RAM) {
    Instruction *compiledInstructions = malloc(100 * sizeof(Instruction));
    compiledInstructions = toCompile(instructions);

    int PC = 0, opcode = MAX_VALUE;

    while (opcode != -1) {
        Instruction inst = compiledInstructions[PC];
        opcode = inst.opCode;
        interpretedMachine(inst, RAM);
        PC++;
    }
}

void interpretedMachine(Instruction inst, int *RAM) {
    int opcode = inst.opCode;
    switch (opcode) {
        // SOMAR
        case 0: {
            int add1 = inst.addressOne;
            int add2 = inst.addressTwo;
            // buscar na RAM
            int contentRAM1 = RAM[add1];
            int contentRAM2 = RAM[add2];
            int sum = contentRAM1 + contentRAM2;
            // salvando resultado na RAM
            int add3 = inst.addressThree;
            RAM[add3] = sum;

            printf("somando %d com %d e gerando %d!\n", contentRAM1,
                   contentRAM2, sum);
            break;
        }
        // SUBTRAIR
        case 1: {
            int add1 = inst.addressOne;
            int add2 = inst.addressTwo;
            // buscar na RAM
            int contentRAM1 = RAM[add1];
            int contentRAM2 = RAM[add2];
            int sum = contentRAM1 - contentRAM2;
            // salvando resultado na RAM
            int add3 = inst.addressThree;
            RAM[add3] = sum;

            printf("subtraindo %d com %d e gerando %d!\n", contentRAM1,
                   contentRAM2, sum);
            break;
        }
        // levar para RAM
        case 2: {
            int content = inst.addressOne;
            int add = inst.addressTwo;
            RAM[add] = content;
            break;
        }
        // trazer da memoriaDados
        case 3: {
            inst.addressOne = RAM[inst.addressTwo];
            break;
        }
    }
}

void createProgramMultiply(int multiplicand, int multiplier, int *RAM) {
    // 0 => somar
    // 1 => sub
    // 2 => levar para memoriaDados
    // 3 => trazer da memoriaDados
    //-1 => halt

    // 3 x 400 = 3 + 3 + 3 + 3 + .... + 3 => 400 vezes
    // opcode | add1 | add2 | add3

    Instruction *multiplyInstructions =
        malloc((multiplier + 3) * sizeof(Instruction));

    Instruction inst;

    inst.opCode = 2;
    inst.addressOne = multiplicand;
    inst.addressTwo = 0;
    inst.addressThree = -1;
    multiplyInstructions[0] = inst;

    inst.opCode = 2;
    inst.addressOne = 0;
    inst.addressTwo = 1;
    inst.addressThree = -1;
    multiplyInstructions[1] = inst;

    for (int i = 2; i < multiplier + 2; i++) {
        inst.opCode = 0;
        inst.addressOne = 0;
        inst.addressTwo = 1;
        inst.addressThree = 1;
        multiplyInstructions[i] = inst;
    }

    // inserindo a ultima instrucao do programa que faz o HALT
    inst.opCode = -1;
    inst.addressOne = -1;
    inst.addressTwo = -1;
    inst.addressThree = -1;
    multiplyInstructions[multiplier + 2] = inst;

    machine(multiplyInstructions, RAM);
}

void createProgramDivide(int dividend, int divisor) {}