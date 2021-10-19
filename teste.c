#include <stdio.h>

float raiz_quadrada(float numero);

int main(int argc, char** argv)
{
    float numero;

    printf("Entre com um número positivo por favor : ");
    scanf("%f", &numero);

    printf("A raiz quadrada de %.3f é %.5f \n", numero, raiz_quadrada(numero));

    return(0);
}


float raiz_quadrada(float numero)
{
    int n;
    float recorre = numero;

    for (n = 0; n < 10; ++n) {
        recorre = (recorre / 2) + numero / (2 * recorre);
        printf("%f\n", recorre);
    }
    return(recorre);
}

void createProgramSquareRoot(double number, double* RAM) {

    double rec, fnumber, resultmult, resultDiv1, resultDiv2;

    Instruction* sqrtInstruction = malloc((3) * sizeof(Instruction));

    Instruction inst;

    inst.opCode = 2;
    inst.addressOne = number;
    inst.addressTwo = 104;
    inst.addressThree = -1;
    sqrtInstruction[0] = inst;
    //Levar para a RAM[104]
    //numero

    inst.opCode = 2;
    inst.addressOne = number;
    inst.addressTwo = 100;
    inst.addressThree = -1;
    sqrtInstruction[1] = inst;
    //Levar para a RAM[100]
    //recorre


    inst.opCode = -1;
    inst.addressOne = -1;
    inst.addressTwo = -1;
    inst.addressThree = -1;
    sqrtInstruction[2] = inst;
    // HALT

    machine(sqrtInstruction, RAM);

    for (int n = 0; n < 10; ++n)
    {
        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 100;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        rec = inst.addressOne;

        createProgramDivide(rec, 2, RAM);

        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 500;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        resultDiv1 = inst.addressOne;

        createProgramMultiply(rec, 2, RAM);

        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 1;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        resultmult = inst.addressOne;

        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 104;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        fnumber = inst.addressOne;


        createProgramDivide(fnumber, resultmult, RAM);

        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 500;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);
        resultDiv2 = inst.addressOne;

        createProgramSum(resultDiv1, resultDiv2, RAM);

        inst.opCode = 3;
        inst.addressOne = -1;
        inst.addressTwo = 2;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);

        inst.opCode = 2;
        inst.addressOne = inst.addressOne;
        inst.addressTwo = 100;
        inst.addressThree = -1;
        interpretedMachine(&inst, RAM);



    }
    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 100;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    //trazer da RAM[100]



    printf(MAGENTA("\nA raiz quadrada de %.0lf é: %.2lf\n\n"), fnumber, inst.addressOne);
}


double x = 0;
double y = number;

for (int i = 0; number > 0; i += 2) {

    inst.opCode = 2;
    inst.addressOne = number;
    inst.addressTwo = 200;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);

    inst.opCode = 2;
    inst.addressOne = i;
    inst.addressTwo = 201;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);

    inst.opCode = 1;
    inst.addressOne = 200;
    inst.addressTwo = 201;
    inst.addressThree = 203;
    interpretedMachine(&inst, RAM);


    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 203;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    number = inst.addressOne;


    inst.opCode = 2;
    inst.addressOne = x;
    inst.addressTwo = 200;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);


    inst.opCode = 2;
    inst.addressOne = 1;
    inst.addressTwo = 201;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);


    inst.opCode = 0;
    inst.addressOne = 200;
    inst.addressTwo = 201;
    inst.addressThree = 203;
    interpretedMachine(&inst, RAM);


    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 203;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    x = inst.addressOne;

}

if (number < 0) {

    inst.opCode = 2;
    inst.addressOne = x;
    inst.addressTwo = 200;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);



    inst.opCode = 2;
    inst.addressOne = 1;
    inst.addressTwo = 201;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);



    inst.opCode = 1;
    inst.addressOne = 200;
    inst.addressTwo = 201;
    inst.addressThree = 203;
    interpretedMachine(&inst, RAM);



    inst.opCode = 3;
    inst.addressOne = -1;
    inst.addressTwo = 203;
    inst.addressThree = -1;
    interpretedMachine(&inst, RAM);
    x = inst.addressOne;



    printf("resultado da raiz quadrada e %d\n\n", x);
}


}