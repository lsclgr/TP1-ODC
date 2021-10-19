#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "basicOperations.h"
#include "machine.h"

// cores e formato de texto
#define ANSI_RESET "\x1b[0m"  // desativa os efeitos anteriores
#define ANSI_BOLD "\x1b[1m"   // coloca o texto em negrito
#define ANSI_COLOR_BLACK "\x1b[30m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_WHITE "\x1b[37m"

// macros para facilitar o uso
#define BOLD(string) ANSI_BOLD string ANSI_RESET
#define BLACK(string) ANSI_COLOR_BLACK string ANSI_RESET
#define BLUE(string) ANSI_COLOR_BLUE string ANSI_RESET
#define RED(string) ANSI_COLOR_RED string ANSI_RESET
#define GREEN(string) ANSI_COLOR_GREEN string ANSI_RESET
#define YELLOW(string) ANSI_COLOR_YELLOW string ANSI_RESET
#define BLUE(string) ANSI_COLOR_BLUE string ANSI_RESET
#define MAGENTA(string) ANSI_COLOR_MAGENTA string ANSI_RESET
#define CYAN(string) ANSI_COLOR_CYAN string ANSI_RESET
#define WHITE(string) ANSI_COLOR_WHITE string ANSI_RESET

void createProgramExponential(double base, double exponent, double* RAM) {
    // 2^4 = 2 x 2 x 2 x 2 => 4 vezes
    // opcode | add1 | add2 | add3

    Instruction inst;
    double baseConvert, result, realBase;

    inst.opCode = 2;
    inst.addressOne = base;
    inst.addressTwo = 700;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);

    if (base < 0) {
        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 700;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        realBase = inst.addressOne;
        createProgramSum(inst.addressOne, inst.addressOne, RAM);

        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 2;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        baseConvert = inst.addressOne;

        createProgramSub(base, baseConvert, RAM);

        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 2;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        base = inst.addressOne;
    }

    if (base == 0) {
        inst.opCode = 2;
        inst.addressOne = 1;
        inst.addressTwo = 1;
        inst.addressThree = -1;
        // base na RAM[0]
        interpretedMachine(&inst, RAM);
    } else {
        inst.opCode = 2;
        inst.addressOne = base;
        inst.addressTwo = 0;
        inst.addressThree = -1;
        // base na RAM[0]
        interpretedMachine(&inst, RAM);

        inst.opCode = 2;
        inst.addressOne = 1;
        inst.addressTwo = 1;
        inst.addressThree = -1;
        // base na RAM[1]
        interpretedMachine(&inst, RAM);

        for (int i = 1; i < exponent + 1; i++) {
            createProgramMultiply(inst.addressOne, base, RAM);

            // trazer da RAM[1]
            inst.opCode = 3;
            inst.addressOne = -1;
            inst.addressTwo = 1;
            inst.addressThree = -1;
            interpretedMachine(&inst, RAM);
            result = inst.addressOne;
        }
    }

    if ((int)exponent % 2 != 0 && baseConvert < 0) {
        createProgramSum(inst.addressOne, inst.addressOne, RAM);

        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 2;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        baseConvert = inst.addressOne;

        createProgramSub(result, baseConvert, RAM);

        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 2;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        result = inst.addressOne;
    }

    inst.opCode = 2;
    inst.addressOne = result;
    inst.addressTwo = 2;
    inst.addressThree = -1;
    // base na RAM[1]
    interpretedMachine(&inst, RAM);

    printf(CYAN(BOLD("\nO resultado de %.2lf elevado a %.0lf é: %.0lf")) "\n\n",
           realBase, exponent, result);
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
    // a na RAM[10]

    inst.opCode = 2;
    inst.addressOne = b;
    inst.addressTwo = 11;
    inst.addressThree = -1;
    deltaInstructions[1] = inst;
    // b na RAM[11]

    inst.opCode = 2;
    inst.addressOne = c;
    inst.addressTwo = 12;
    inst.addressThree = -1;
    deltaInstructions[2] = inst;
    // c na RAM[12]

    createProgramExponential(b, 2, RAM);

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 1;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    int b2 = inst.addressOne;

    inst.opCode = 2;
    inst.addressOne = b2;
    inst.addressTwo = 13;
    inst.addressThree = -1;
    deltaInstructions[3] = inst;
    // b² na RAM[13]

    createProgramMultiply(4, a, RAM);

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 1;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    int n = inst.addressOne;

    inst.opCode = 2;
    inst.addressOne = n;
    inst.addressTwo = 14;
    inst.addressThree = -1;
    deltaInstructions[4] = inst;
    // 4xa na RAM[4]

    createProgramMultiply(n, c, RAM);

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 1;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    n = inst.addressOne;

    inst.opCode = 2;
    inst.addressOne = n;
    inst.addressTwo = 14;
    inst.addressThree = -1;
    deltaInstructions[5] = inst;
    // 4xaxc na RAM[5]

    inst.opCode = 1;
    inst.addressOne = 13;
    inst.addressTwo = 14;
    inst.addressThree = 15;
    deltaInstructions[8] = inst;
    // subtraindo b²-(4ac)

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

    printf(CYAN(BOLD("\nO resultado de delta é: %.2lf")) "\n\n",
           inst.addressOne);
    free(deltaInstructions);
}

void createProgramFactorial(double number, double* RAM) {
    // 5! = 5 . 4 . 3 . 2 . 1 = 120
    //  opcode | add1 | add2 | add3

    Instruction* factorialInstruction = malloc((5) * sizeof(Instruction));

    Instruction inst;
    double result, initNumb;

    inst.opCode = 2;
    inst.addressOne = number;
    inst.addressTwo = 4;
    inst.addressThree = -1;
    factorialInstruction[3] = inst;
    // número para a RAM[3]

    inst.opCode = 2;
    inst.addressOne = number;
    inst.addressTwo = 0;
    inst.addressThree = -1;
    factorialInstruction[0] = inst;
    // número para a RAM[0]

    inst.opCode = 2;
    inst.addressOne = 1;
    inst.addressTwo = 1;
    inst.addressThree = -1;
    factorialInstruction[1] = inst;
    // result para a RAM[1]

    inst.opCode = 2;
    inst.addressOne = 1;
    inst.addressTwo = 2;
    inst.addressThree = -1;
    factorialInstruction[2] = inst;
    // contador de dec. para a RAM[2]

    inst.opCode = -1;
    inst.addressOne = -1;
    inst.addressTwo = -1;
    inst.addressThree = -1;
    factorialInstruction[4] = inst;
    // HALT

    machine(factorialInstruction, RAM);

    for (int i = (number - 1); i > 0; i--) {
        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 0;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        number = inst.addressOne;

        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 1;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        result = inst.addressOne;

        createProgramMultiply(number, result, RAM);

        inst.opCode = 1;
        inst.addressOne = 0;
        inst.addressTwo = 2;
        inst.addressThree = 0;
        interpretedMachine(&inst, RAM);
    }

    // trazer da RAM[4]
    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 4;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    initNumb = inst.addressOne;

    // trazer da RAM[1]
    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 1;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);

    printf("\n%.0lf! é: %.0lf\n\n", initNumb, inst.addressOne);
    free(factorialInstruction);
}

void createProgramSquareRoot(double number, double* RAM) {
    double precision = 0.000001, initNumb, comparation, divResult, numberDiv2,
           numberDiv22;

    Instruction* sqrtInstruction = malloc((4) * sizeof(Instruction));

    Instruction inst;

    inst.opCode = 2;
    inst.addressOne = number;
    inst.addressTwo = 104;
    inst.addressThree = -1;
    sqrtInstruction[2] = inst;
    // Levar para a RAM[104]

    inst.opCode = 2;
    inst.addressOne = number;
    inst.addressTwo = 100;
    inst.addressThree = -1;
    sqrtInstruction[0] = inst;
    // Levar para a RAM[100]

    inst.opCode = 2;
    inst.addressOne = 1;
    inst.addressTwo = 102;
    inst.addressThree = -1;
    sqrtInstruction[1] = inst;
    // Levar para a RAM[102]

    inst.opCode = -1;
    inst.addressOne = -1;
    inst.addressTwo = -1;
    inst.addressThree = -1;
    sqrtInstruction[3] = inst;
    // HALT

    machine(sqrtInstruction, RAM);

    inst.opCode = 1;
    inst.addressOne = 100;
    inst.addressTwo = 102;
    inst.addressThree = 103;
    interpretedMachine(&inst, RAM);
    // sub para a comparação

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 103;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    comparation = inst.addressOne;
    // trazer da RAM[103]

    while (comparation >= precision) {
        inst.opCode = 0;
        inst.addressOne = 102;
        inst.addressTwo = 100;
        inst.addressThree = 106;
        interpretedMachine(&inst, RAM);
        // (a + b)

        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 106;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        // trazer RAM[106]

        createProgramDivide(inst.addressOne, 2, RAM);

        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 500;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        divResult = inst.addressOne;
        // trazer result da div RAM[3]

        inst.opCode = 2;
        inst.addressOne = divResult;
        inst.addressTwo = 100;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        // mandar para RAM[100];

        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 104;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        numberDiv2 = inst.addressOne;
        // tranzendo RAM[104]

        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 100;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        numberDiv22 = inst.addressOne;
        printf("\n\n%lf\n\n", numberDiv22);

        // trazendo RAM[100]

        createProgramDivide(numberDiv2, numberDiv22, RAM);

        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 500;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        divResult = inst.addressOne;
        // trazer result da div RAM[3]

        inst.opCode = 2;
        inst.addressOne = divResult;
        inst.addressTwo = 102;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        // levar para RAM[102]

        inst.opCode = 1;
        inst.addressOne = 100;
        inst.addressTwo = 102;
        inst.addressThree = 103;
        interpretedMachine(&inst, RAM);
        // sub para a comparação

        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 103;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        comparation = inst.addressOne;
        // trazer da RAM[103]
    }

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 104;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    initNumb = inst.addressOne;
    // trazer da RAM[104]

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 100;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);

    printf(MAGENTA("\nA raiz quadrada de %.0lf é: %.2lf\n\n"), initNumb,
           inst.addressOne);
}

void createProgramSquareRoot2(double number, double* RAM) {
    Instruction inst;

    double x = 0;

    for (int i = 0; number > 0; i += 2) {
        inst.opCode = 2;
        inst.addressOne = number;
        inst.addressTwo = 400;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);

        inst.opCode = 2;
        inst.addressOne = i;
        inst.addressTwo = 401;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);

        inst.opCode = 1;
        inst.addressOne = 400;
        inst.addressTwo = 401;
        inst.addressThree = 403;
        interpretedMachine(&inst, RAM);

        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 403;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        number = inst.addressOne;

        inst.opCode = 2;
        inst.addressOne = x;
        inst.addressTwo = 400;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);

        inst.opCode = 2;
        inst.addressOne = 1;
        inst.addressTwo = 401;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);

        inst.opCode = 0;
        inst.addressOne = 400;
        inst.addressTwo = 401;
        inst.addressThree = 403;
        interpretedMachine(&inst, RAM);

        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 403;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        x = inst.addressOne;
    }

    if (number < 0) {
        inst.opCode = 2;
        inst.addressOne = x;
        inst.addressTwo = 400;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);

        inst.opCode = 2;
        inst.addressOne = 1;
        inst.addressTwo = 401;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);

        inst.opCode = 1;
        inst.addressOne = 400;
        inst.addressTwo = 401;
        inst.addressThree = 403;
        interpretedMachine(&inst, RAM);

        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 403;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        x = inst.addressOne;

        printf("resultado da raiz quadrada e %lf\n\n", x);
    }
}

void createProgramBhaskara(double a, double b, double c, double* RAM) {
    Instruction* bhaskaraInstructions = malloc((10) * sizeof(Instruction));

    Instruction inst;

    inst.opCode = 2;
    inst.addressOne = a;
    inst.addressTwo = 200;
    inst.addressThree = -1;
    bhaskaraInstructions[0] = inst;
    // a na RAM[10]

    inst.opCode = 2;
    inst.addressOne = -(b);
    inst.addressTwo = 201;
    inst.addressThree = -1;
    bhaskaraInstructions[1] = inst;
    // b na RAM[201]

    inst.opCode = 2;
    inst.addressOne = c;
    inst.addressTwo = 202;
    inst.addressThree = -1;
    bhaskaraInstructions[2] = inst;
    // c na RAM[202]

    inst.opCode = -1;
    inst.addressOne = -1;
    inst.addressTwo = -1;
    inst.addressThree = -1;
    bhaskaraInstructions[3] = inst;
    // HALT

    machine(bhaskaraInstructions, RAM);

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 200;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    a = inst.addressOne;
    // trazer da RAM[200]

    createProgramMultiply(2, a, RAM);

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 1;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    double a2 = inst.addressOne;
    // trazer da RAM[1]

    inst.opCode = 2;
    inst.addressOne = a2;
    inst.addressTwo = 203;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    // 2xa na RAM[203]

    createProgramDelta(a, b, c, RAM);

    // trazer da RAM[15]
    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 15;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    double delta = inst.addressOne;

    inst.opCode = 2;
    inst.addressOne = delta;
    inst.addressTwo = 204;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    // delta na RAM[204]

    if (delta >= 0) {
        createProgramSquareRoot2(delta, RAM);

        // trazer da RAM[100]
        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 403;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        double squareRoot = inst.addressOne;
        printf("\n%lf\n", squareRoot);

        inst.opCode = 2;
        inst.addressOne = squareRoot;
        inst.addressTwo = 205;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        // raiz na RAM[205]

        inst.opCode = 0;
        inst.addressOne = 201;
        inst.addressTwo = 205;
        inst.addressThree = 206;
        interpretedMachine(&inst, RAM);
        //-b+raiz(delta) na RAM[206]

        inst.opCode = 1;
        inst.addressOne = 201;
        inst.addressTwo = 205;
        inst.addressThree = 207;
        interpretedMachine(&inst, RAM);
        //-b-raiz(delta) na RAM[207]

        // trazer da RAM[206]
        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 206;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        double x1 = inst.addressOne;

        createProgramDivide(x1, a2, RAM);

        // trazer da RAM[500] resultado div
        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 500;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        x1 = inst.addressOne;

        // x1 na RAM[206]
        inst.opCode = 2;
        inst.addressOne = x1;
        inst.addressTwo = 206;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);

        // trazer da RAM[207]
        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 207;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        double x2 = inst.addressOne;

        createProgramDivide(x2, a2, RAM);

        // trazer da RAM[500] resultado div
        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 500;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        x2 = inst.addressOne;

        // x2 na RAM[207]
        inst.opCode = 2;
        inst.addressOne = x2;
        inst.addressTwo = 207;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);

        if (delta == 0) {
            printf("O resultado de x é: %.1lf", x1);
        } else {
            printf("O resultado de x1 é: %.1lf\nO resultado de x2 é: %.1lf\n",
                   x1, x2);
        }
    } else {
        printf("\nNão é possível calcular bhaskara!\n");
    }
    free(bhaskaraInstructions);
}

void createProgramIntAnglesSum(double n, double* RAM) {
    // s=(n-1)x180

    Instruction inst;

    inst.opCode = 2;
    inst.addressOne = n;
    inst.addressTwo = 250;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    // n na RAM[250]

    inst.opCode = 2;
    inst.addressOne = 2;
    inst.addressTwo = 251;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    // n na RAM[250]

    inst.opCode = 1;
    inst.addressOne = 250;
    inst.addressTwo = 251;
    inst.addressThree = 252;
    interpretedMachine(&inst, RAM);
    // n-2 na RAM[251]

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 252;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    double result = inst.addressOne;

    createProgramMultiply(result, 180, RAM);

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 1;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    result = inst.addressOne;

    printf("\nA soma dos angulos internos do polígono de %.0lf lados é: %.0lf ",
           n, result);
}

void createProgramAP(double n, int a1, int an, double* RAM) {
    //((a1+an)*n)/2

    Instruction* apInstructions = malloc(10 * sizeof(Instruction));

    Instruction inst;

    inst.opCode = 2;
    inst.addressOne = n;
    inst.addressTwo = 300;
    inst.addressThree = -1;
    apInstructions[0] = inst;
    // n na RAM[300]

    inst.opCode = 2;
    inst.addressOne = a1;
    inst.addressTwo = 301;
    inst.addressThree = -1;
    apInstructions[1] = inst;
    // a1 na RAM[301]

    inst.opCode = 2;
    inst.addressOne = an;
    inst.addressTwo = 302;
    inst.addressThree = -1;
    apInstructions[2] = inst;
    // n na RAM[302]

    inst.opCode = 0;
    inst.addressOne = 301;
    inst.addressTwo = 302;
    inst.addressThree = 303;
    apInstructions[3] = inst;
    // a1+an na RAM[303]

    createProgramDivide(n, 2, RAM);

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 500;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    double resultDiv = inst.addressOne;
    // pegar result divisão

    inst.opCode = 2;
    inst.addressOne = resultDiv;
    inst.addressTwo = 304;
    inst.addressThree = -1;
    apInstructions[4] = inst;
    // n/2 na RAM[304]

    inst.opCode = -1;
    inst.addressOne = -1;
    inst.addressTwo = -1;
    inst.addressThree = -1;
    apInstructions[5] = inst;
    // HALT

    machine(apInstructions, RAM);

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 303;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    double resultSum = inst.addressOne;
    // pegar result soma

    createProgramMultiply(resultDiv, resultSum, RAM);

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 1;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    double result = inst.addressOne;
    // pegar result multiply

    inst.opCode = 2;
    inst.addressOne = result;
    inst.addressTwo = 305;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    // resultado na RAM[305]

    printf("\nO resultado da soma da PA é: %.2lf\n", result);

    free(apInstructions);
}

void createProgramSquareArea(double n, double* RAM) {
    Instruction inst;

    inst.opCode = 2;
    inst.addressOne = n;
    inst.addressTwo = 30;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);

    createProgramMultiply(n, n, RAM);

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 1;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    double area = inst.addressOne;

    inst.opCode = 2;
    inst.addressOne = area;
    inst.addressTwo = 31;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);

    printf(YELLOW("\nA área do quadrado é: %.2lf\n\n"), area);
}

void createProgramRectangleArea(double l, double h, double* RAM) {
    Instruction inst;

    inst.opCode = 2;
    inst.addressOne = l;
    inst.addressTwo = 40;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);

    inst.opCode = 2;
    inst.addressOne = h;
    inst.addressTwo = 41;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);

    createProgramMultiply(l, h, RAM);

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 1;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    double area = inst.addressOne;

    inst.opCode = 2;
    inst.addressOne = area;
    inst.addressTwo = 42;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);

    printf(YELLOW("\nA área do retângulo é: %.2lf\n\n"), area);
}

void createProgramTriangleArea(double l, double h, double* RAM) {
    Instruction inst;

    inst.opCode = 2;
    inst.addressOne = l;
    inst.addressTwo = 50;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);

    inst.opCode = 2;
    inst.addressOne = h;
    inst.addressTwo = 51;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);

    createProgramMultiply(l, h, RAM);

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 1;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    double area = inst.addressOne;

    inst.opCode = 2;
    inst.addressOne = area;
    inst.addressTwo = 52;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);

    createProgramDivide(area, 2, RAM);

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 500;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    area = inst.addressOne;

    inst.opCode = 2;
    inst.addressOne = area;
    inst.addressTwo = 52;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);

    printf(YELLOW("\nA área do triângulo é: %.2lf\n\n"), area);
}

void createProgramCircleArea(double r, double* RAM) {
    Instruction inst;

    inst.opCode = 2;
    inst.addressOne = r;
    inst.addressTwo = 55;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);

    inst.opCode = 2;
    inst.addressOne = 3.1415;
    inst.addressTwo = 56;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);

    createProgramExponential(r, 2, RAM);

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 2;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    double area = inst.addressOne;

    inst.opCode = 2;
    inst.addressOne = area;
    inst.addressTwo = 57;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 56;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    double pi = inst.addressOne;

    createProgramMultiply(pi, area, RAM);

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 1;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    area = inst.addressOne;

    inst.opCode = 2;
    inst.addressOne = area;
    inst.addressTwo = 57;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);

    printf(YELLOW("\nA área do circulo é: %.2lf\n\n"), area);
}

void createProgramFibonacci(int num1, double* RAM) {
    int count;
    int fibonacci[num1];
    fibonacci[0] = 0;
    fibonacci[1] = 1;

    Instruction* fibonacciInstructions = malloc((5) * sizeof(Instruction));

    for (count = 2; count < num1; count++) {
        // Basicamente fibo = n - 1 + n - 2. e é.
        // Começo do n-1.
        Instruction inst;
        // Levando count pra posiçao 2
        inst.opCode = 2;
        inst.addressOne = count;
        inst.addressTwo = 2;
        inst.addressThree = -1;
        fibonacciInstructions[0] = inst;

        // Levando 1 pra posiçao 3
        inst.opCode = 2;
        inst.addressOne = 1;
        inst.addressTwo = 3;
        inst.addressThree = -1;
        fibonacciInstructions[1] = inst;

        // Subtraindo count com 1
        inst.opCode = 1;
        inst.addressOne = 2;
        inst.addressTwo = 3;
        inst.addressThree = 3;
        fibonacciInstructions[2] = inst;

        // HALT
        inst.opCode = -1;
        inst.addressOne = -1;
        inst.addressTwo = -1;
        inst.addressThree = -1;
        fibonacciInstructions[3] = inst;

        machine(fibonacciInstructions, RAM);

        // Trazendo o resultado da RAM
        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 3;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        int subOne = inst.addressOne;

        // Começo do n -2.
        inst.opCode = 2;
        inst.addressOne = count;
        inst.addressTwo = 2;
        inst.addressThree = -1;
        fibonacciInstructions[0] = inst;

        // Armazenando 2 na posição 3
        inst.opCode = 2;
        inst.addressOne = 2;
        inst.addressTwo = 3;
        inst.addressThree = -1;
        fibonacciInstructions[1] = inst;

        // subtraindo o contador por 2
        inst.opCode = 1;
        inst.addressOne = 2;
        inst.addressTwo = 3;
        inst.addressThree = 3;
        fibonacciInstructions[2] = inst;

        // HALT
        inst.opCode = -1;
        inst.addressOne = -1;
        inst.addressTwo = -1;
        inst.addressThree = -1;
        fibonacciInstructions[3] = inst;

        machine(fibonacciInstructions, RAM);

        // Trazendo da RAM
        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 3;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        int subTwo = inst.addressOne;

        inst.opCode = 2;
        inst.addressOne = subOne;
        inst.addressTwo = 2;
        inst.addressThree = -1;
        fibonacciInstructions[0] = inst;

        inst.opCode = 2;
        inst.addressOne = subTwo;
        inst.addressTwo = 3;
        inst.addressThree = -1;
        fibonacciInstructions[1] = inst;

        inst.opCode = 0;
        inst.addressOne = 2;
        inst.addressTwo = 3;
        inst.addressThree = 3;
        fibonacciInstructions[2] = inst;

        // HALT
        inst.opCode = -1;
        inst.addressOne = -1;
        inst.addressTwo = -1;
        inst.addressThree = -1;
        fibonacciInstructions[3] = inst;

        machine(fibonacciInstructions, RAM);

        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 3;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);

        fibonacci[count] = inst.addressOne;
    }
    printf("\n\n");
    for (int i = 0; i < num1; i++) {
        printf("%d ", fibonacci[i]);
    }
    printf("\n\n");
    free(fibonacciInstructions);
}
