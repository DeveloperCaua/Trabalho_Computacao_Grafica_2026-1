#ifndef VIEWPORT_H
#define VIEWPORT_H

#define nLinha 50
#define nColuna 100

int **criaTela(int larg, int alt);
void limpaTela(int **tela);
void imprimeTela(int **tela);
void ndcToViewport(int **tela, float x1, float y1, float x2, float y2);

#endif