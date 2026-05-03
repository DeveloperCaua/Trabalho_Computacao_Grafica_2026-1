#include <stdio.h>
#include <stdlib.h>
#include "viewport.h"

void desenhaLinha(int **tela, int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1, sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2, e2;

    while (1) {
        if(x0 >= 0 && x0 < nColuna && y0 >= 0 && y0 < nLinha) {
            tela[y0][x0] = 1;
        }
        if (x0 == x1 && y0 == y1) break;
        e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }
}

void ndcToViewport(int **tela, float x1, float y1, float x2, float y2) {
    int xa = nColuna * ((x1 + 1.0) / 2.0);
    int xb = nColuna * ((x2 + 1.0) / 2.0);
    int ya = nLinha * ((-y1 + 1.0) / 2.0);
    int yb = nLinha * ((-y2 + 1.0) / 2.0);

    desenhaLinha(tela, xa, ya, xb, yb);
}

int **criaTela(int larg, int alt) {
    int **tela, i, j;
    tela = (int **) malloc(alt * sizeof(int *));
    for(i=0; i<alt; i++) {
        tela[i] = (int *) malloc(larg * sizeof(int));
        for(j=0; j<larg; j++) tela[i][j] = 0;
    }
    return tela;
}

void limpaTela(int **tela) {
    for(int i = 0; i < nLinha; i++) {
        for(int j = 0; j < nColuna; j++) {
            tela[i][j] = 0;
        }
    }
}

void imprimeTela(int **tela) {
    int i, j;
    printf(" X|0000000000111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999|\n");
    printf("Y |0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789|\n");
    printf("--+----------------------------------------------------------------------------------------------------+\n");
    for(i=0; i<nLinha; i++) {
        printf("%2d|", i);
        for(j=0; j<nColuna; j++)
            if(tela[i][j]) printf("*");
            else printf(" ");
        printf("|\n");
    }
    printf("--+----------------------------------------------------------------------------------------------------+\n");
}
