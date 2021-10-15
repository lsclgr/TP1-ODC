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

int* createRAM(int* RAM);
void createRandomInstructions(int* RAM);
void machine(Instruction* instructions, int* RAM);
void interpretedMachine(Instruction* inst, int* RAM);
Instruction* toCompile(Instruction* instructions);
int createProgramMultiply(int multiplicand, int multiplier, int* RAM);
int createProgramDivide(int dividend, int divisor, int* RAM);
int createProgramExponential(int base, int exponent, int* RAM);
int createProgramDelta(int a, int b, int c, int* RAM);

int main() {
    srand(time(NULL));
    int* RAM = NULL;
    RAM = createRAM(RAM);

    createRAM(RAM);
    //createRandomInstructions(RAM);
    int x = createProgramMultiply(400, 3, RAM);
    int y = createProgramDivide(12, 3, RAM);
    int z = createProgramExponential(2, 4, RAM);
    int delta = createProgramDelta(1, 5, 0, RAM);

    x = y = z = delta;
    y = x;

    return 0;
}

int* createRAM(int* RAM) {
    RAM = (int*)malloc(1000 * sizeof(int));

    for (int i = 0; i < 1000; i++) {
        RAM[i] = rand() % 100;
    }
    return RAM;
}

void createRandomInstructions(int* RAM) {
    // 01|22|13|45 => isto é uma instrução
    // 00|33|12|01 => isto é outra instrução

    // 0 => opcode => somar
    // 1 => opcode => subtrair
    //-1 => halt -> parar
    Instruction* instructions = malloc(100 * sizeof(Instruction));
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
Instruction* toCompile(Instruction* instructions) {
    // aqui teria o q o GCC faz, que demorou décadas para ser feito
    // eficientemente
    return instructions;
}

void machine(Instruction* instructions, int* RAM) {
    Instruction* compiledInstructions = malloc(100 * sizeof(Instruction));
    compiledInstructions = toCompile(instructions);

    int PC = 0, opcode = MAX_VALUE;

    while (opcode != -1) {
        Instruction inst = compiledInstructions[PC];
        opcode = inst.opCode;
        interpretedMachine(&inst, RAM);
        PC++;
    }
}

void interpretedMachine(Instruction* inst, int* RAM) {
    int opcode = inst->opCode;
    switch (opcode) {
        // SOMAR
    case 0: {
        int add1 = inst->addressOne;
        int add2 = inst->addressTwo;
        // buscar na RAM
        int contentRAM1 = RAM[add1];
        int contentRAM2 = RAM[add2];
        int sum = contentRAM1 + contentRAM2;
        // salvando resultado na RAM
        int add3 = inst->addressThree;
        RAM[add3] = sum;

        printf("somando %d com %d e gerando %d!\n", contentRAM1, contentRAM2, sum);
        break;
    }
          // SUBTRAIR
    case 1: {
        int add1 = inst->addressOne;
        int add2 = inst->addressTwo;
        // buscar na RAM
        int contentRAM1 = RAM[add1];
        int contentRAM2 = RAM[add2];
        int sum = contentRAM1 - contentRAM2;
        // salvando resultado na RAM
        int add3 = inst->addressThree;
        RAM[add3] = sum;

        printf("subtraindo %d com %d e gerando %d!\n", contentRAM1, contentRAM2, sum);
        break;
    }
          // levar para RAM
    case 2: {
        int content = inst->addressOne;
        int add = inst->addressTwo;
        RAM[add] = content;
        break;
    }
          // trazer da RAM
    case 3: {
        inst->addressOne = RAM[inst->addressTwo];
        break;
    }
    }
}

int createProgramMultiply(int multiplicand, int multiplier, int* RAM) {
    // 0 => somar
    // 1 => sub
    // 2 => levar para memoriaDados
    // 3 => trazer da memoriaDados
    //-1 => halt

    // 3 x 400 = 3 + 3 + 3 + 3 + .... + 3 => 400 vezes
    // opcode | add1 | add2 | add3

    Instruction* multiplyInstructions = malloc((multiplier + 3) * sizeof(Instruction));

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

    // trazer da RAM[1]
    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 1;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    int result = inst.addressOne;

    printf("\nO resultado da multiplicação de %d por %d é: %d\n\n", multiplicand, multiplier, result);

    return result;
}

int createProgramDivide(int dividend, int divisor, int* RAM) {
    // 0 => somar
    // 1 => sub
    // 2 => levar para memoriaDados
    // 3 => trazer da memoriaDados
    //-1 => halt

    // 12 / 3 = (12-3); (9-3); (6-3); (3-3); (0-3) => 4
    // 15 / 4 = (15-4); (11-4); (7-4); (3-4) => 3

    // monto um programa apenas para levar os dados para RAM
    Instruction* divInstructions = malloc(5 * sizeof(Instruction));

    Instruction inst;
    inst.opCode = 2;
    inst.addressOne = dividend;
    inst.addressTwo = 0;
    inst.addressThree = -1;
    divInstructions[0] = inst;
    // RAM[0] = dividend

    inst.opCode = 2;
    inst.addressOne = divisor;
    inst.addressTwo = 1;
    inst.addressThree = -1;
    divInstructions[1] = inst;
    // RAM[1] = divisor

    inst.opCode = 2;
    inst.addressOne = 1;
    inst.addressTwo = 2;
    inst.addressThree = -1;
    divInstructions[2] = inst;
    // RAM[2] = 1
    // representa uma variável de incremento

    inst.opCode = 2;
    inst.addressOne = 0;
    inst.addressTwo = 3;
    inst.addressThree = -1;
    divInstructions[3] = inst;
    // RAM[3] = 0
    // representa quantas subtrações foram feitas
    // representa o resultado da divisão

    inst.opCode = -1;
    inst.addressOne = -1;
    inst.addressTwo = -1;
    inst.addressThree = -1;
    divInstructions[4] = inst;

    machine(divInstructions, RAM);

    // trazer da RAM[0]
    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 0;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    dividend = inst.addressOne;

    // trazer da memoriaDados[1]
    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 1;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    divisor = inst.addressOne;

    // 12 / 3 = (12-3); (9-3); (6-3); (3-3); (0-3) => 4
    // 15 / 4 = (15-4); (11-4); (7-4); (3-4) => 3

    while (dividend >= divisor) {
        // subtrair
        inst.opCode = 1;
        inst.addressOne = 0;
        inst.addressTwo = 1;
        inst.addressThree = 0;
        interpretedMachine(&inst, RAM);

        // somar
        inst.opCode = 0;
        inst.addressOne = 2;
        inst.addressTwo = 3;
        inst.addressThree = 3;
        interpretedMachine(&inst, RAM);

        // trazer da RAM[0]
        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 0;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        dividend = inst.addressOne;
    }
    // trazer da RAM[3]
    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 3;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    int result = inst.addressOne;

    printf("\nO resultado da divisão de %d por %d é: %d\n\n", dividend, divisor, result);

    return result;
}
int createProgramExponential(int base, int exponent, int* RAM) {

    // 2^4 = 2 x 2 x 2 x 2 => 4 vezes
    // opcode | add1 | add2 | add3

    Instruction* expInstructions = malloc((exponent + 2) * sizeof(Instruction));

    Instruction inst;

    int result = 1;

    inst.opCode = 2;
    inst.addressOne = base;
    inst.addressTwo = 0;
    inst.addressThree = -1;
    expInstructions[0] = inst;
    //base na RAM[0]


    for (int i = 1; i < exponent + 1; i++) {
        result = createProgramMultiply(result, base, RAM);
        inst.opCode = 2;
        inst.addressOne = result;
        inst.addressTwo = 1;
        inst.addressThree = -1;
        expInstructions[1] = inst;
        //resultado na RAM[1]
    }

    // inserindo a ultima instrucao do programa que faz o HALT
    inst.opCode = -1;
    inst.addressOne = -1;
    inst.addressTwo = -1;
    inst.addressThree = -1;
    expInstructions[exponent + 2] = inst;

    printf("\nO resultado de %d elevado a %d é: %d\n\n", base, exponent, result);

    return result;
}

int createProgramDelta(int a, int b, int c, int* RAM) {
    // 0 => somar
        // 1 => sub
        // 2 => levar para memoriaDados
        // 3 => trazer da memoriaDados
        //-1 => halt

        // 2^4 = 2 x 2 x 2 x 2 => 4 vezes
        // opcode | add1 | add2 | add3

    Instruction* deltaInstructions = malloc((11) * sizeof(Instruction));

    Instruction inst;

    int result = 0;

    inst.opCode = 2;
    inst.addressOne = a;
    inst.addressTwo = 10;
    inst.addressThree = -1;
    deltaInstructions[0] = inst;
    //a na RAM[0]

    inst.opCode = 2;
    inst.addressOne = b;
    inst.addressTwo = 11;
    inst.addressThree = -1;
    deltaInstructions[1] = inst;
    //b na RAM[1]

    inst.opCode = 2;
    inst.addressOne = c;
    inst.addressTwo = 12;
    inst.addressThree = -1;
    deltaInstructions[2] = inst;
    //c na RAM[2]

    inst.opCode = 2;
    inst.addressOne = createProgramExponential(b, 2, RAM);
    inst.addressTwo = 13;
    inst.addressThree = -1;
    deltaInstructions[3] = inst;
    //b² na RAM[3]

    inst.opCode = 2;
    inst.addressOne = createProgramMultiply(4, a, RAM);
    inst.addressTwo = 14;
    inst.addressThree = -1;
    deltaInstructions[4] = inst;
    //4xa na RAM[4]

    inst.opCode = 2;
    inst.addressOne = createProgramMultiply(deltaInstructions[4].addressOne, c, RAM);
    inst.addressTwo = 14;
    inst.addressThree = -1;
    deltaInstructions[5] = inst;
    //4xaxc na RAM[5]

    inst.opCode = 1;
    inst.addressOne = deltaInstructions[3].addressOne;
    inst.addressTwo = deltaInstructions[5].addressOne;
    inst.addressThree = 15;
    deltaInstructions[8] = inst;
    //subtraindo b²-(4ac)

    // trazer da RAM[16]
    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 16;
    inst.addressThree = -1;
    deltaInstructions[9] = inst;
    result = inst.addressOne;

    // inserindo a ultima instrucao do programa que faz o HALT
    inst.opCode = -1;
    inst.addressOne = -1;
    inst.addressTwo = -1;
    inst.addressThree = -1;
    deltaInstructions[10] = inst;

    machine(deltaInstructions, RAM);

    printf("\nO resultado de delta é: %d\n\n", result);
    return result;
}