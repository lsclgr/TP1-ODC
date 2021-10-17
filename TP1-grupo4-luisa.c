#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_VALUE 100000

// cores e formato de texto
#define ANSI_RESET            "\x1b[0m"  // desativa os efeitos anteriores
#define ANSI_BOLD             "\x1b[1m"  // coloca o texto em negrito
#define ANSI_COLOR_BLACK      "\x1b[30m"
#define ANSI_COLOR_RED        "\x1b[31m"
#define ANSI_COLOR_GREEN      "\x1b[32m"
#define ANSI_COLOR_YELLOW     "\x1b[33m"
#define ANSI_COLOR_BLUE       "\x1b[34m"
#define ANSI_COLOR_MAGENTA    "\x1b[35m"
#define ANSI_COLOR_CYAN       "\x1b[36m"
#define ANSI_COLOR_WHITE      "\x1b[37m"
#define ANSI_BG_COLOR_BLACK   "\x1b[40m"
#define ANSI_BG_COLOR_RED     "\x1b[41m"
#define ANSI_BG_COLOR_GREEN   "\x1b[42m"
#define ANSI_BG_COLOR_YELLOW  "\x1b[43m"
#define ANSI_BG_COLOR_BLUE    "\x1b[44m"
#define ANSI_BG_COLOR_MAGENTA "\x1b[45m"
#define ANSI_BG_COLOR_CYAN    "\x1b[46m"
#define ANSI_BG_COLOR_WHITE   "\x1b[47m"

// macros para facilitar o uso
#define BOLD(string)       ANSI_BOLD             string ANSI_RESET
#define BLACK(string)      ANSI_COLOR_BLACK      string ANSI_RESET
#define BLUE(string)       ANSI_COLOR_BLUE       string ANSI_RESET
#define RED(string)        ANSI_COLOR_RED        string ANSI_RESET
#define GREEN(string)      ANSI_COLOR_GREEN      string ANSI_RESET
#define YELLOW(string)     ANSI_COLOR_YELLOW     string ANSI_RESET
#define BLUE(string)       ANSI_COLOR_BLUE       string ANSI_RESET
#define MAGENTA(string)    ANSI_COLOR_MAGENTA    string ANSI_RESET
#define CYAN(string)       ANSI_COLOR_CYAN       string ANSI_RESET
#define WHITE(string)      ANSI_COLOR_WHITE      string ANSI_RESET
#define BG_BLACK(string)   ANSI_BG_COLOR_BLACK   string ANSI_RESET
#define BG_BLUE(string)    ANSI_BG_COLOR_BLUE    string ANSI_RESET
#define BG_RED(string)     ANSI_BG_COLOR_RED     string ANSI_RESET
#define BG_GREEN(string)   ANSI_BG_COLOR_GREEN   string ANSI_RESET
#define BG_YELLOW(string)  ANSI_BG_COLOR_YELLOW  string ANSI_RESET
#define BG_BLUE(string)    ANSI_BG_COLOR_BLUE    string ANSI_RESET
#define BG_MAGENTA(string) ANSI_BG_COLOR_MAGENTA string ANSI_RESET
#define BG_CYAN(string)    ANSI_BG_COLOR_CYAN    string ANSI_RESET
#define BG_WHITE(string)   ANSI_BG_COLOR_WHITE   string ANSI_RESET

typedef struct {
    int opCode;
    double addressOne;
    double addressTwo;
    double addressThree;
} Instruction;

double* createRAM(double* RAM);
void createRandomInstructions(double* RAM);
void machine(Instruction* instructions, double* RAM);
void interpretedMachine(Instruction* inst, double* RAM);
Instruction* toCompile(Instruction* instructions);
void createProgramSum(double num1, double num2, double* RAM);
void createProgramSub(double num1, double num2, double* RAM);
void createProgramMultiply(double multiplicand, double multiplier, double* RAM);
void createProgramDivide(double dividend, double divisor, double* RAM);
void createProgramExponential(double base, double exponent, double* RAM);
void createProgramFactorial(double number, double* RAM);
void createProgramDelta(double a, double b, double c, double* RAM);

int main() {
    srand(time(NULL));
    int op = 0;
    double* RAM = NULL, num1, num2, num3;
    RAM = createRAM(RAM);
    Instruction inst;

    createRAM(RAM);
    //createRandomInstructions(RAM);

    printf("Escolha a opção:\n"
        "1 - Soma\n"
        "2 - Subtração\n"
        "3 - Multiplicação\n"
        "4 - Divisão\n"
        "5 - Exponencial\n"
        "6 - Delta\n"
        "8 - Fatorial\n");
    scanf("%d", &op);

    switch (op)
    {
    case 1:
        printf("Informe o primeiro valor: ");
        scanf("%lf", &num1);

        printf("Informe o segundo valor: ");
        scanf("%lf", &num2);

        createProgramSum(num1, num2, RAM);

        break;
    case 2:
        printf("Informe o primeiro valor: ");
        scanf("%lf", &num1);

        printf("Informe o segundo valor: ");
        scanf("%lf", &num2);

        createProgramSub(num1, num2, RAM);

        break;

    case 3:
        printf("Informe o multiplicando: ");
        scanf("%lf", &num1);

        printf("Informe o multiplicador: ");
        scanf("%lf", &num2);

        createProgramMultiply(num1, num2, RAM);

        break;

    case 4:
        printf("Informe o dividendo: ");
        scanf("%lf", &num1);

        printf("Informe o divisor: ");
        scanf("%lf", &num2);

        createProgramDivide(num1, num2, RAM);

        break;

    case 5:
        printf("Informe a base: ");
        scanf("%lf", &num1);

        printf("Informe o expoente: ");
        scanf("%lf", &num2);

        createProgramExponential(num1, num2, RAM);

        break;

    case 6:
        printf("Informe o valor de A: ");
        scanf("%lf", &num1);

        printf("Informe o valor de B: ");
        scanf("%lf", &num2);

        printf("Informe o valor de C: ");
        scanf("%lf", &num3);

        createProgramDelta(num1, num2, num3, RAM);

        break;

    default:
        break;
    }
    return 0;
}

double* createRAM(double* RAM) {
    RAM = (double*)malloc(1000 * sizeof(double));

    for (int i = 0; i < 1000; i++) {
        RAM[i] = rand() % 100;
    }
    return RAM;
}

void createRandomInstructions(double* RAM) {
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

void machine(Instruction* instructions, double* RAM) {
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

void interpretedMachine(Instruction* inst, double* RAM) {
    int opcode = inst->opCode;
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

        printf(GREEN("somando %.1lf com %.1lf e gerando %.1lf!")"\n", contentRAM1, contentRAM2, sum);
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

        printf(RED("subtraindo %.1lf com %.1lf e gerando %.1lf!")"\n", contentRAM1, contentRAM2, sub);
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

void createProgramSum(double num1, double num2, double* RAM) {
    Instruction* sumInstructions = malloc((4) * sizeof(Instruction));

    Instruction inst;

    inst.opCode = 2;
    inst.addressOne = num1;
    inst.addressTwo = 0;
    inst.addressThree = -1;
    sumInstructions[0] = inst;

    inst.opCode = 2;
    inst.addressOne = num2;
    inst.addressTwo = 1;
    inst.addressThree = -1;
    sumInstructions[1] = inst;

    inst.opCode = 0;
    inst.addressOne = 0;
    inst.addressTwo = 1;
    inst.addressThree = 2;
    sumInstructions[2] = inst;

    inst.opCode = -1;
    inst.addressOne = -1;
    inst.addressTwo = -1;
    inst.addressThree = -1;
    sumInstructions[3] = inst;

    machine(sumInstructions, RAM);

    //trazer da RAM[2]
    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 2;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);

    printf(CYAN(BOLD("\nO resultado da soma de %.1lf e %.1lf é: %.1lf"))"\n\n", num1, num2, inst.addressOne);

}

void createProgramSub(double num1, double num2, double* RAM) {
    Instruction* subInstructions = malloc((4) * sizeof(Instruction));

    Instruction inst;

    inst.opCode = 2;
    inst.addressOne = num1;
    inst.addressTwo = 0;
    inst.addressThree = -1;
    subInstructions[0] = inst;

    inst.opCode = 2;
    inst.addressOne = num2;
    inst.addressTwo = 1;
    inst.addressThree = -1;
    subInstructions[1] = inst;


    inst.opCode = 1;
    inst.addressOne = 0;
    inst.addressTwo = 1;
    inst.addressThree = 2;
    subInstructions[2] = inst;

    inst.opCode = -1;
    inst.addressOne = -1;
    inst.addressTwo = -1;
    inst.addressThree = -1;
    subInstructions[3] = inst;

    machine(subInstructions, RAM);

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 2;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);

    printf(CYAN(BOLD("\nO resultado da subtração de %.1lf e %.1lf é: %.1lf"))"\n\n", num1, num2, inst.addressOne);
}

void createProgramMultiply(double multiplicand, double multiplier, double* RAM) {
    // 0 => somar
    // 1 => sub
    // 2 => levar para memoriaDados
    // 3 => trazer da memoriaDados
    //-1 => halt

    // 3 x 400 = 3 + 3 + 3 + 3 + .... + 3 => 400 vezes
    // opcode | add1 | add2 | add3

    Instruction* multiplyInstructions = malloc(((int)multiplier + 3) * sizeof(Instruction));

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

    for (int i = 2; i < (int)multiplier + 2; i++) {
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
    multiplyInstructions[(int)multiplier + 2] = inst;

    machine(multiplyInstructions, RAM);

    // trazer da RAM[1]
    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 1;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);

    printf(BLUE("\nMultiplicando %.2lf e %.2lf e gerando: %.2lf")"\n\n", multiplicand, multiplier, inst.addressOne);
}

void createProgramDivide(double dividend, double divisor, double* RAM) {
    // 0 => somar
    // 1 => sub
    // 2 => levar para memoriaDados
    // 3 => trazer da memoriaDados
    //-1 => halt

    // 12 / 3 = (12-3); (9-3); (6-3); (3-3); (0-3) => 4
    // 15 / 4 = (15-4); (11-4); (7-4); (3-4) => 3

    // monto um programa apenas para levar os dados para RAM
    Instruction* divInstructions = malloc(6 * sizeof(Instruction));

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

    inst.opCode = 2;
    inst.addressOne = dividend;
    inst.addressTwo = 4;
    inst.addressThree = -1;
    divInstructions[4] = inst;
    // RAM[4] = dividend

    inst.opCode = -1;
    inst.addressOne = -1;
    inst.addressTwo = -1;
    inst.addressThree = -1;
    divInstructions[5] = inst;

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
    // trazer da RAM[4]
    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 4;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    dividend = inst.addressOne;

    // trazer da RAM[3]
    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 3;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);


    printf(YELLOW("\nDividindo %.2lf por %.2lf e gerando: %.2lf")"\n\n", dividend, divisor, inst.addressOne);

}
void createProgramExponential(double base, double exponent, double* RAM) {

    // 2^4 = 2 x 2 x 2 x 2 => 4 vezes
    // opcode | add1 | add2 | add3

    Instruction inst;

    inst.opCode = 2;
    inst.addressOne = base;
    inst.addressTwo = 0;
    inst.addressThree = -1;
    //base na RAM[0]
    interpretedMachine(&inst, RAM);

    inst.opCode = 2;
    inst.addressOne = 1;
    inst.addressTwo = 1;
    inst.addressThree = -1;
    //base na RAM[1]
    interpretedMachine(&inst, RAM);


    for (int i = 1; i < exponent + 1; i++) {
        createProgramMultiply(inst.addressOne, base, RAM);

        // trazer da RAM[1]
        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 1;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
    }

    printf(CYAN(BOLD("\nO resultado de %.2lf elevado a %.0lf é: %.0lf"))"\n\n", base, exponent, inst.addressOne);

}

void createProgramDelta(double a, double b, double c, double* RAM) {
    // 0 => somar
        // 1 => sub
        // 2 => levar para memoriaDados
        // 3 => trazer da memoriaDados
        //-1 => halt

        // 2^4 = 2 x 2 x 2 x 2 => 4 vezes
        // opcode | add1 | add2 | add3

    Instruction* deltaInstructions = malloc((10) * sizeof(Instruction));

    Instruction inst;

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
    //inst.addressOne = createProgramExponential(b, 2, RAM);
    inst.addressTwo = 13;
    inst.addressThree = -1;
    deltaInstructions[3] = inst;
    //b² na RAM[3]

    inst.opCode = 2;
    //inst.addressOne = createProgramMultiply(4, a, RAM);
    inst.addressTwo = 14;
    inst.addressThree = -1;
    deltaInstructions[4] = inst;
    //4xa na RAM[4]

    inst.opCode = 2;
    //inst.addressOne = createProgramMultiply(deltaInstructions[4].addressOne, c, RAM);
    inst.addressTwo = 14;
    inst.addressThree = -1;
    deltaInstructions[5] = inst;
    //4xaxc na RAM[5]

    inst.opCode = 1;
    inst.addressOne = 13;
    inst.addressTwo = 14;
    inst.addressThree = 15;
    deltaInstructions[8] = inst;
    //subtraindo b²-(4ac)

    // inserindo a ultima instrucao do programa que faz o HALT
    inst.opCode = -1;
    inst.addressOne = -1;
    inst.addressTwo = -1;
    inst.addressThree = -1;
    deltaInstructions[9] = inst;

    machine(deltaInstructions, RAM);

    // trazer da RAM[15]
    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 15;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);

    printf(CYAN(BOLD("\nO resultado de delta é: %.2lf"))"\n\n", inst.addressOne);
}