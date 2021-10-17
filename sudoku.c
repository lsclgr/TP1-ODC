#include<stdio.h>
#define N 9

int grade[N][N] =
{ { 0, 6, 0, 1, 0, 4, 0, 5,  0},
 { 0, 0, 8, 3, 0, 5, 6, 0, 0},
 { 2, 0, 0, 0, 0, 0, 0, 0, 1},
 { 8, 0, 0, 4, 0, 7, 0, 0, 6},
 { 0, 0, 6, 0, 0, 0, 3, 0, 0},
 { 7, 0, 0, 9, 0, 1, 0, 0, 4},
 { 5, 0, 0, 0, 0, 0, 0, 0, 2},
 { 0, 0, 7, 2, 0, 6, 9, 0, 0},
 { 0, 4, 0, 5, 0, 8, 0, 7, 0} };

void desenha_grade() {
    int i, j;
    printf("+---------+---------+---------+\n");
    for (i = 0; i < N; i++) {
        printf("|");
        for (j = 0; j < N; j++) {
            if (grade[i][j] != 0)
                printf(" %d ", grade[i][j]);
            else
                printf("   ");
            if (j % 3 == 2)
                printf("|");
        }
        if (i % 3 == 2)
            printf("\n+---------+---------+---------+");
        printf("\n");
    }
}

int tenta_colocar(int i, int j, int k) {
    int c, l;
    for (l = 0; l < N; l++)
        if (grade[l][j] == k)
            return 0;
    for (c = 0; c < N; c++)
        if (grade[i][c] == k)
            return 0;
    for (l = i - i % 3; l < i - i % 3 + 3; l++)
        for (c = j - j % 3; c < j - j % 3 + 3; c++)
            if (grade[l][c] == k)
                return 0;
    grade[i][j] = k;
    desenha_grade();
    return 1;
}

int completa_grade(int i, int j) {
    int k;

    if (i == 9)  /* Grade completa */
        return 1;

    if (grade[i][j] != 0) { /* Pré-definida */
        if (j == 8)
            return completa_grade(i + 1, 0);
        else
            return completa_grade(i, j + 1);
    }


    for (k = 1; k <= 9; k++)
        if (tenta_colocar(i, j, k)) {
            if (j == 8) {
                if (completa_grade(i + 1, 0))
                    return 1;
            }
            else
                if (completa_grade(i, j + 1))
                    return 1;
        }
    grade[i][j] = 0;
    return 0;
}

int main() {
    desenha_grade();
    completa_grade(0,0);
    desenha_grade();
    return 0;
}