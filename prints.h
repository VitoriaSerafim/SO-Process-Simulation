#include <stdio.h>


void imprimirGrafico(int n) {
    // Imprimir cabeçalho
    printf("-");
    for (int i = 0; i <= n+1; ++i) {
        printf("----");
    }
    printf("\n");

    // Imprimir conteúdo
    for (int i = 1; i <= n; ++i) {
        printf("P%d ",i);
        printf("|");
        for (int j = 1; j <= n; ++j) {
            printf(" *** |", (i - 1) * n + j);
        }
        printf("\n");

        // Imprimir linha divisória entre as linhas
        printf("-");
        for (int k = 0; k <= n +1; ++k) {
            printf("----");
        }
        printf("\n");
    }
}

