#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "machine.h"
#include "basicOperations.h"

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

void createProgramExponential(double base, double exponent, double* RAM) {

    // 2^4 = 2 x 2 x 2 x 2 => 4 vezes
    // opcode | add1 | add2 | add3

    Instruction inst;
    if (base == 0) {
        inst.opCode = 2;
        inst.addressOne = 1;
        inst.addressTwo = 1;
        inst.addressThree = -1;
        //base na RAM[0]
        interpretedMachine(&inst, RAM);
    }
    else {
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
    //a na RAM[10]

    inst.opCode = 2;
    inst.addressOne = b;
    inst.addressTwo = 11;
    inst.addressThree = -1;
    deltaInstructions[1] = inst;
    //b na RAM[11]

    inst.opCode = 2;
    inst.addressOne = c;
    inst.addressTwo = 12;
    inst.addressThree = -1;
    deltaInstructions[2] = inst;
    //c na RAM[12]

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
    //b² na RAM[13]

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
    //4xa na RAM[4]

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
    free(deltaInstructions);
}

void createProgramFactorial(double number, double* RAM) {

    //5! = 5 . 4 . 3 . 2 . 1 = 120
    // opcode | add1 | add2 | add3

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
}

void createProgramSquareRoot(double number, double* RAM) {

    double precision = 0.000001, initNumb, comparation, divResult, numberDiv2, numberDiv22;

    Instruction* sqrtInstruction = malloc((4) * sizeof(Instruction));

    Instruction inst;

    inst.opCode = 2;
    inst.addressOne = number;
    inst.addressTwo = 104;
    inst.addressThree = -1;
    sqrtInstruction[2] = inst;
    //Levar para a RAM[104]

    inst.opCode = 2;
    inst.addressOne = number;
    inst.addressTwo = 100;
    inst.addressThree = -1;
    sqrtInstruction[0] = inst;
    //Levar para a RAM[100]

    inst.opCode = 2;
    inst.addressOne = 1;
    inst.addressTwo = 102;
    inst.addressThree = -1;
    sqrtInstruction[1] = inst;
    //Levar para a RAM[102]

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
    //sub para a comparação

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 103;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    comparation = inst.addressOne;
    //trazer da RAM[103]

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
        //trazer RAM[106]

        createProgramDivide(inst.addressOne, 2, RAM);

        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 500;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        divResult = inst.addressOne;
        //trazer result da div RAM[3]

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
        // trazendo RAM[104]

        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 100;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        numberDiv22 = inst.addressOne;
        printf("\n\n%lf\n\n", numberDiv22);

        //trazendo RAM[100]

        createProgramDivide(numberDiv2, numberDiv22, RAM);

        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 500;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        divResult = inst.addressOne;
        //trazer result da div RAM[3]

        inst.opCode = 2;
        inst.addressOne = divResult;
        inst.addressTwo = 102;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        //levar para RAM[102]

        inst.opCode = 1;
        inst.addressOne = 100;
        inst.addressTwo = 102;
        inst.addressThree = 103;
        interpretedMachine(&inst, RAM);
        //sub para a comparação

        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 103;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        comparation = inst.addressOne;
        //trazer da RAM[103]

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

    printf(MAGENTA("\nA raiz quadrada de %.0lf é: %.2lf\n\n"), initNumb, inst.addressOne);
    free(sqrtInstruction);
}

void createProgramBhaskara(double a, double b, double c, double* RAM) {
    Instruction* bhaskaraInstructions = malloc((10) * sizeof(Instruction));

    Instruction inst;

    inst.opCode = 2;
    inst.addressOne = a;
    inst.addressTwo = 200;
    inst.addressThree = -1;
    bhaskaraInstructions[0] = inst;
    //a na RAM[10]

    inst.opCode = 2;
    inst.addressOne = -b;
    inst.addressTwo = 201;
    inst.addressThree = -1;
    bhaskaraInstructions[1] = inst;
    //b na RAM[11]

    inst.opCode = 2;
    inst.addressOne = c;
    inst.addressTwo = 202;
    inst.addressThree = -1;
    bhaskaraInstructions[2] = inst;
    //c na RAM[12]

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 200;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    a = inst.addressOne;
    // trazer da RAM[104]

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
    bhaskaraInstructions[3] = inst;
    //2xa na RAM[203]

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
    bhaskaraInstructions[4] = inst;
    //delta na RAM[204]

    if (delta >= 0) {
        createProgramSquareRoot(delta, RAM);

        // trazer da RAM[100]
        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 100;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        double squareRoot = inst.addressOne;

        inst.opCode = 2;
        inst.addressOne = squareRoot;
        inst.addressTwo = 205;
        inst.addressThree = -1;
        bhaskaraInstructions[5] = inst;
        //raiz na RAM[205]

        inst.opCode = 0;
        inst.addressOne = 201;
        inst.addressTwo = 205;
        inst.addressThree = 206;
        bhaskaraInstructions[6] = inst;
        //-b+raiz(delta) na RAM[206]

        inst.opCode = 1;
        inst.addressOne = 201;
        inst.addressTwo = 205;
        inst.addressThree = 207;
        bhaskaraInstructions[7] = inst;
        //-b-raiz(delta) na RAM[207]

        machine(bhaskaraInstructions, RAM);

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

        //x1 na RAM[206]
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

        //x2 na RAM[207]
        inst.opCode = 2;
        inst.addressOne = x2;
        inst.addressTwo = 207;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);

        if (delta == 0) {
            printf("O resultado de x é: %.1lf", x1);
        }
        else {
            printf("O resultado de x1 é: %.1lf\nO resultado de x2 é: %.1lf\n", x1, x2);
        }
    }
    else {
        printf("\nNão é possível calcular bhaskara!\n");
    }
    free(bhaskaraInstructions);

}

void createProgramIntAnglesSum(double n, double* RAM) {
    //s=(n-1)x180

    Instruction inst;

    inst.opCode = 2;
    inst.addressOne = n;
    inst.addressTwo = 250;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    //n na RAM[250]

    inst.opCode = 1;
    inst.addressOne = n;
    inst.addressTwo = 2;
    inst.addressThree = 251;
    interpretedMachine(&inst, RAM);
    //n-2 na RAM[251]

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 251;
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

    printf("\nA soma dos angulos internos do polígono de %.0lf lados é: %.0lf ", n, result);

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
    //n na RAM[300]

    inst.opCode = 2;
    inst.addressOne = a1;
    inst.addressTwo = 301;
    inst.addressThree = -1;
    apInstructions[1] = inst;
    //a1 na RAM[301]

    inst.opCode = 2;
    inst.addressOne = an;
    inst.addressTwo = 302;
    inst.addressThree = -1;
    apInstructions[2] = inst;
    //n na RAM[302]

    inst.opCode = 0;
    inst.addressOne = 301;
    inst.addressTwo = 302;
    inst.addressThree = 303;
    apInstructions[3] = inst;
    //a1+an na RAM[303]

    createProgramDivide(n, 2, RAM);

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 500;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    double resultDiv = inst.addressOne;
    //pegar result divisão

    inst.opCode = 2;
    inst.addressOne = resultDiv;
    inst.addressTwo = 304;
    inst.addressThree = -1;
    apInstructions[4] = inst;
    //n/2 na RAM[304]

    machine(apInstructions, RAM);

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 304;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    double resultSum = inst.addressOne;
    //pegar result soma

    createProgramMultiply(resultDiv, resultSum, RAM);

    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 1;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    double result = inst.addressOne;
    //pegar result multiply

    inst.opCode = 2;
    inst.addressOne = result;
    inst.addressTwo = 305;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    //resultado na RAM[305]

    printf("\nO resultado da soma da PA é: %.2lf\n", result);

    free(apInstructions);
}