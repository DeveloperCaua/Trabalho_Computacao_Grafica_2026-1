#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "objeto.h"
#include "algebra.h"
#define PI 3.14159265358979323846f

//L� as informa��es de um arquivo e as carrega num novo objeto alocado
tObjeto3d *carregaObjeto(char *nomeArquivo){
	FILE *f = fopen(nomeArquivo, "r");
	if(!f) return NULL;

	tObjeto3d *obj = (tObjeto3d*) malloc(sizeof(tObjeto3d));
	if(!obj){ fclose(f); return NULL; }

	if(fscanf(f, "%d", &obj->nPontos) != 1){ fclose(f); free(obj); return NULL; }

	obj->pontos = (float**) malloc(sizeof(float*) * obj->nPontos);
	for(int i = 0; i < obj->nPontos; i++){
		obj->pontos[i] = (float*) malloc(sizeof(float)*4);
		float x,y,z;
		fscanf(f, "%f %f %f", &x, &y, &z);
		obj->pontos[i][0] = x;
		obj->pontos[i][1] = y;
		obj->pontos[i][2] = z;
		obj->pontos[i][3] = 1.0f;
	}

	if(fscanf(f, "%d", &obj->nArestas) != 1){
		obj->nArestas = 0;
	}
	obj->arestas = (int**) malloc(sizeof(int*) * obj->nArestas);
	for(int i = 0; i < obj->nArestas; i++){
		obj->arestas[i] = (int*) malloc(sizeof(int)*2);
		int a,b;
		fscanf(f, "%d %d", &a, &b);
		obj->arestas[i][0] = a;
		obj->arestas[i][1] = b;
	}

	obj->transl = (float*) malloc(sizeof(float)*3);
	obj->escala = (float*) malloc(sizeof(float)*3);
	obj->transl[0] = obj->transl[1] = obj->transl[2] = 0.0f;
	obj->escala[0] = obj->escala[1] = obj->escala[2] = 1.0f;
	obj->Rx = obj->Ry = obj->Rz = 0.0f;

	obj->modelMatrix = (float**) malloc(sizeof(float*)*4);
	for(int i = 0; i < 4; i++){
		obj->modelMatrix[i] = (float*) malloc(sizeof(float)*4);
	}
	criaIdentidade4d(obj->modelMatrix);

	fclose(f);
	return obj;
}

//Altera a modelMatrix de um objeto para redimenciona-lo segundo os par�metros escalaX, escalaY e escalaZ
void escalaObjeto(tObjeto3d *objeto, float escalaX, float escalaY, float escalaZ){
	float **S = (float**) malloc(sizeof(float*)*4);
	for(int i = 0; i <4; i++) S[i] = (float*) malloc(sizeof(float)*4);
	criaIdentidade4d(S);
	S[0][0] = escalaX;
	S[1][1] = escalaY;
	S[2][2] = escalaZ;

	multMatriz4d(S, objeto->modelMatrix);

	for(int i=0;i<4;i++) free(S[i]); free(S);
}

//Altera a modelMatrix de um objeto para translada-lo segundo os par�metros transX, transY e transZ
void transladaObjeto(tObjeto3d *objeto, float transX, float transY, float transZ){
	float **T = (float**) malloc(sizeof(float*)*4);
	for(int i = 0; i <4; i++) T[i] = (float*) malloc(sizeof(float)*4);
	criaIdentidade4d(T);
	T[0][3] = transX;
	T[1][3] = transY;
	T[2][3] = transZ;

	multMatriz4d(T, objeto->modelMatrix);

	for(int i=0;i<4;i++) free(T[i]); free(T);
}

//Altera a modelMatrix de um objeto para rotaciona-lo ao redor do eixo X segundo o angulo informado
void rotacionaObjetoEixoX(tObjeto3d *objeto, float angulo){
	float r = angulo * PI / 180.0f;
	float **R = (float**) malloc(sizeof(float*)*4);
	for(int i = 0; i <4; i++) R[i] = (float*) malloc(sizeof(float)*4);
	criaIdentidade4d(R);
	R[1][1] = cosf(r);
	R[1][2] = -sinf(r);
	R[2][1] = sinf(r);
	R[2][2] = cosf(r);

	multMatriz4d(R, objeto->modelMatrix);

	for(int i=0;i<4;i++) free(R[i]); free(R);
}

//Altera a modelMatrix de um objeto para rotaciona-lo ao redor do eixo Y segundo o angulo informado
void rotacionaObjetoEixoY(tObjeto3d *objeto, float angulo){
	float r = angulo * PI / 180.0f;
	float **R = (float**) malloc(sizeof(float*)*4);
	for(int i = 0; i <4; i++) R[i] = (float*) malloc(sizeof(float)*4);
	criaIdentidade4d(R);
	R[0][0] = cosf(r);
	R[0][2] = sinf(r);
	R[2][0] = -sinf(r);
	R[2][2] = cosf(r);

	multMatriz4d(R, objeto->modelMatrix);

	for(int i=0;i<4;i++) free(R[i]); free(R);
}

//Altera a modelMatrix de um objeto para rotaciona-lo ao redor do eixo Z segundo o angulo informado
void rotacionaObjetoEixoZ(tObjeto3d *objeto, float angulo){
	float r = angulo * PI / 180.0f;
	float **R = (float**) malloc(sizeof(float*)*4);
	for(int i = 0; i <4; i++) R[i] = (float*) malloc(sizeof(float)*4);
	criaIdentidade4d(R);
	R[0][0] = cosf(r);
	R[0][1] = -sinf(r);
	R[1][0] = sinf(r);
	R[1][1] = cosf(r);

	multMatriz4d(R, objeto->modelMatrix);

	for(int i=0;i<4;i++) free(R[i]); free(R);
}

//Imprime um objeto no terminal
void imprimeObjetoDBG(tObjeto3d *objeto){
	if(!objeto) return;
	printf("Objeto: %d pontos, %d arestas\n", objeto->nPontos, objeto->nArestas);
	for(int i = 0; i < objeto->nPontos; i++){
		printf("%d: %f %f %f\n", i, objeto->pontos[i][0], objeto->pontos[i][1], objeto->pontos[i][2]);
	}
}

//Desaloca o objeto
void desalocaObjeto(tObjeto3d *objeto){
	if(!objeto) return;
	for(int i = 0; i < objeto->nPontos; i++){
		free(objeto->pontos[i]);
	}
	free(objeto->pontos);
	for(int i = 0; i < objeto->nArestas; i++){
		free(objeto->arestas[i]);
	}
	free(objeto->arestas);
	free(objeto->transl);
	free(objeto->escala);
	for(int i = 0; i < 4; i++){
		free(objeto->modelMatrix[i]);
	}
	free(objeto->modelMatrix);
	free(objeto);
}

