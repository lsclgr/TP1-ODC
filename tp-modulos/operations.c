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


void createProgramFactorial(double number, double* RAM) {

    //5! = 5 . 4 . 3 . 2 . 1 = 120
    // opcode | add1 | add2 | add3

    Instruction* factorialInstruction = malloc((4) * sizeof(Instruction));

    Instruction inst;
    double result;

    printf("\n\n617\n\n");

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
    factorialInstruction[3] = inst;
    // HALT

    machine(factorialInstruction, RAM);

    printf("\n\n649\n\n");

    for (int i = (number - 1); i > 0; i--) {
        printf("\n\n652\n\n");
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
    // trazer da RAM[1]
    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 1;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);

    printf("\n%.0lf! é: %.0lf\n\n" ,number, inst.addressOne);
}