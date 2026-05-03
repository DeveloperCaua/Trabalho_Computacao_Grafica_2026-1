#ifndef OBJETO_H
#define OBJETO_H

typedef struct {
    int n;
    float **vertices;
    float tx, ty;
    float sx, sy;
    float theta;
} Objeto;

Objeto* carregarArquivo(const char* nomeArquivo);
void transformaObj(int **tela, Objeto *obj);
void desalocaObjeto(Objeto *obj);

#endif