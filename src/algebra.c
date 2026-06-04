#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Ajusta valores de um matriz 4d para uma matriz identidade
void criaIdentidade4d(float **novaMatriz){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			if(i == j) novaMatriz[i][j] = 1.0f;
			else novaMatriz[i][j] = 0.0f;
		}
	}
}

//Imprime uma matriz 4d no terminal
void imprimeMatriz4dDBG(float **matriz){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			printf("%8.4f ", matriz[i][j]);
		}
		printf("\n");
	}
}

//Multiplica uma matriz 4d por um ponto
float *multMatriz4dPonto(float **matriz, float *ponto){
	float *res = (float*) malloc(sizeof(float)*4);
	for(int i = 0; i < 4; i++){
		res[i] = 0.0f;
		for(int j = 0; j < 4; j++){
			res[i] += matriz[i][j] * ponto[j];
		}
	}
	return res;
}

//Multiplica duas matrizes 4d salvando o resultado na segunda matriz
void multMatriz4d(float **matrizA, float **matrizB){
	float temp[4][4];
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			temp[i][j] = 0.0f;
			for(int k = 0; k < 4; k++){
				temp[i][j] += matrizA[i][k] * matrizB[k][j];
			}
		}
	}
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			matrizB[i][j] = temp[i][j];
		}
	}
}
