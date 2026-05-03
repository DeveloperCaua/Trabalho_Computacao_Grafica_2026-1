#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "objeto.h"
#include "viewport.h"

Objeto* carregarArquivo(const char* nomeArquivo) {
    FILE *arq = fopen(nomeArquivo, "r");
    if (!arq) return NULL;

    Objeto *obj = (Objeto*) malloc(sizeof(Objeto));
    fscanf(arq, "%d", &obj->n);

    obj->vertices = (float**) malloc(obj->n * sizeof(float*));
    for(int i = 0; i < obj->n; i++) {
        obj->vertices[i] = (float*) malloc(2 * sizeof(float));
        fscanf(arq, "%f %f", &obj->vertices[i][0], &obj->vertices[i][1]);
    }

    obj->tx = 0.0; obj->ty = 0.0;
    obj->sx = 1.0; obj->sy = 1.0;
    obj->theta = 0.0;

    fclose(arq);
    return obj;
}

void transformaObj(int **tela, Objeto *obj) {
    for (int i = 0; i < obj->n; i++) {
        float P1x = obj->vertices[i][0];
        float P1y = obj->vertices[i][1];

        int proximo = (i + 1) % obj->n;
        float P2x = obj->vertices[proximo][0];
        float P2y = obj->vertices[proximo][1];

        // Rotação -> Escala -> Translação
        float P1x_m = (P1x * cos(obj->theta) - P1y * sin(obj->theta)) * obj->sx + obj->tx;
        float P1y_m = (P1x * sin(obj->theta) + P1y * cos(obj->theta)) * obj->sy + obj->ty;
        
        float P2x_m = (P2x * cos(obj->theta) - P2y * sin(obj->theta)) * obj->sx + obj->tx;
        float P2y_m = (P2x * sin(obj->theta) + P2y * cos(obj->theta)) * obj->sy + obj->ty;

        ndcToViewport(tela, P1x_m, P1y_m, P2x_m, P2y_m);
    }
}

void desalocaObjeto(Objeto *obj) {
    if (obj) {
        for (int i = 0; i < obj->n; i++) {
            free(obj->vertices[i]);
        }
        free(obj->vertices);
        free(obj);
    }
}