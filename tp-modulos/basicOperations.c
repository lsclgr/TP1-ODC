#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "machine.h"

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
    char stringConvert[10];
    double divResult;

    Instruction inst;
    inst.opCode = 2;
    inst.addressOne = dividend;
    inst.addressTwo = 0;
    inst.addressThree = -1;
    divInstructions[0] = inst;
    // RAM[0] = dividend

    inst.opCode = 2;
    inst.addressOne = divisor;
    inst.addressTwo = 32;
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

    // trazer da RAM[1]
    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 32;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    divisor = inst.addressOne;

    // 12 / 3 = (12-3); (9-3); (6-3); (3-3); (0-3) => 4
    // 15 / 4 = (15-4); (11-4); (7-4); (3-4) => 3

    while (dividend >= divisor) {
        // subtrair
        inst.opCode = 1;
        inst.addressOne = 0;
        inst.addressTwo = 32;
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
    divResult = inst.addressOne;

    char aux = (int)inst.addressOne + '0';
    stringConvert[0] = aux;
    stringConvert[1] = '.';

    if (dividend > 0) {
        // trazer da RAM[0]
        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 0;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        dividend = inst.addressOne;

        for (int i = 2; i < 9;i++) {
            inst.opCode = 2;
            inst.addressOne = 0;
            inst.addressTwo = 3;
            inst.addressThree = -1;
            interpretedMachine(&inst, RAM);

            createProgramMultiply(dividend, 10, RAM);

            // trazer da RAM[1]
            inst.opCode = 3;
            inst.addressOne = -1;
            inst.addressTwo = 1;
            inst.addressThree = -1;
            interpretedMachine(&inst, RAM);
            dividend = inst.addressOne;

            // levar para RAM[0]
            inst.opCode = 2;
            inst.addressOne = dividend;
            inst.addressTwo = 0;
            inst.addressThree = -1;
            interpretedMachine(&inst, RAM);


            while (dividend >= divisor) {
                // subtrair
                inst.opCode = 1;
                inst.addressOne = 0;
                inst.addressTwo = 32;
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
            char aux = (int)inst.addressOne + '0';
            stringConvert[i] = aux;

            // trazer da RAM[0]
            inst.opCode = 3;
            inst.addressOne = -1;
            inst.addressTwo = 0;
            inst.addressThree = -1;
            interpretedMachine(&inst, RAM);
            dividend = inst.addressOne;

            if (dividend == 0) {
                break;
            }
        }
        printf("%s\n\n", stringConvert);
        divResult = atof(stringConvert);
    }

    // levar para a RAM[500]
    inst.opCode = 2;
    inst.addressOne = divResult;
    inst.addressTwo = 500;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);


    // trazer da RAM[4]
    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 4;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    dividend = inst.addressOne;


    printf(YELLOW("\nDividindo %.2lf por %.2lf e gerando: %.5lf\n\n"), dividend, divisor, divResult);

}