#ifndef OBJETO_H
#define OBJETO_H

typedef struct {
    int n;             // Número de vértices
    float **vertices;  // Matriz [n][2] (SCO)
    float tx, ty;      // Translação
    float sx, sy;      // Escala
    float theta;       // Rotação (radianos)
} Objeto;

Objeto* carregarArquivo(const char* nomeArquivo);
void transformaObj(int **tela, Objeto *obj);
void desalocaObjeto(Objeto *obj);

#endif