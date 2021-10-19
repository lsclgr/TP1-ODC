#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "basicOperations.h"
#include "machine.h"
#include "operations.h"
#define MAX_VALUE 100000

// cores e formato de texto
#define ANSI_RESET "\x1b[0m" // desativa os efeitos anteriores
#define ANSI_BOLD "\x1b[1m"  // coloca o texto em negrito
#define ANSI_COLOR_WHITE "\x1b[37m"

// macros para facilitar o uso
#define BOLD(string) ANSI_BOLD string ANSI_RESET
#define WHITE(string) ANSI_COLOR_WHITE string ANSI_RESET

int main()
{
    srand(time(NULL));
    int op = MAX_VALUE, num;
    double *RAM = NULL, num1, num2, num3;
    RAM = createRAM(RAM);

    createRAM(RAM);
    // createRandomInstructions(RAM);

    while (op != 0)
    {
        printf(
            WHITE(BOLD("\nEscolha a opção:\n"
                       "1 - Soma\n"
                       "2 - Subtração\n"
                       "3 - Multiplicação\n"
                       "4 - Divisão\n"
                       "5 - Potenciação\n"
                       "6 - Delta\n"
                       "7 - Fibonacci\n"
                       "8 - Fatorial\n"
                       "9 - Raiz quadrada\n"
                       "10- Bhaskara\n"
                       "11- Soma dos angulos internos\n"
                       "12- Progressão aritmética\n"
                       "13- Area do quadrado\n"
                       "14- Area do retangulo\n"
                       "15- Area do triangulo\n"
                       "16- Area do círculo\n"
                       "0 - SAIR\n")));
        scanf("%d", &op);
        switch (op)
        {
        case 0:

            break;
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
        case 7:
            printf("Informe o número de termos da sequencia: ");
            scanf("%d", &num);

            createProgramFibonacci(num, RAM);

            break;
        case 8:
            printf("Informe o número que você deseja saber o fatorial: ");
            scanf("%lf", &num1);

            createProgramFactorial(num1, RAM);

            break;

        case 9:
            printf(
                "Informe o número que você deseja saber a raiz quadrada: ");
            scanf("%lf", &num1);

            createProgramSquareRoot(num1, RAM);

            break;

        case 10:
            printf("Informe o valor de A: ");
            scanf("%lf", &num1);

            printf("Informe o valor de B: ");
            scanf("%lf", &num2);

            printf("Informe o valor de C: ");
            scanf("%lf", &num3);

            createProgramBhaskara(num1, num2, num3, RAM);

            break;

        case 11:
            printf("Informe o numero de lados: ");
            scanf("%lf", &num1);

            createProgramIntAnglesSum(num1, RAM);

            break;

        case 12:
            printf("Informe o numero de termos da PA: ");
            scanf("%lf", &num1);

            printf("Informe o valor do primeiro termo: ");
            scanf("%lf", &num2);

            printf("Informe o valor do segundo termo: ");
            scanf("%lf", &num3);

            createProgramAP(num1, num2, num3, RAM);

            break;
        case 13:
            printf("Informe o lado do quadrado: ");
            scanf("%lf", &num1);

            createProgramSquareArea(num1, RAM);

            break;
        case 14:
            printf("Informe a base do retangulo: ");
            scanf("%lf", &num1);

            printf("Informe a altura do retangulo: ");
            scanf("%lf", &num2);

            createProgramRectangleArea(num1, num2, RAM);

            break;
        case 15:
            printf("Informe a base do triangulo: ");
            scanf("%lf", &num1);

            printf("Informe a altura do triangulo: ");
            scanf("%lf", &num2);

            createProgramTriangleArea(num1, num2, RAM);

            break;

        case 16:
            printf("Informe o valor do raio: ");
            scanf("%lf", &num1);

            createProgramCircleArea(num1, RAM);

            break;
        default:

            createRandomInstructions(RAM);

            break;
        }
    }
    free(RAM);
    return 0;
}