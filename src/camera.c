#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "camera.h"
#include "algebra.h"

//Aloca a estrutura de mem�ria para representar a c�mera
tCamera3d *criaCamera(){
	tCamera3d *cam = (tCamera3d*) malloc(sizeof(tCamera3d));
	cam->posicao = (float*) malloc(sizeof(float)*3);
	cam->foco = (float*) malloc(sizeof(float)*3);
	cam->cima = (float*) malloc(sizeof(float)*3);
	cam->viewMatrix = (float**) malloc(sizeof(float*)*4);
	for(int i=0;i<4;i++) cam->viewMatrix[i] = (float*) malloc(sizeof(float)*4);
	criaIdentidade4d(cam->viewMatrix);
	return cam;
}

//Recebe os par�metros espec�ficos da c�mera e calcula viewMatrix
void defineCamera(tCamera3d *camera, float posX, float posY, float posZ, float focX, float focY, float focZ, float cimX, float cimY, float cimZ){
	camera->posicao[0] = posX; camera->posicao[1] = posY; camera->posicao[2] = posZ;
	camera->foco[0] = focX; camera->foco[1] = focY; camera->foco[2] = focZ;
	camera->cima[0] = cimX; camera->cima[1] = cimY; camera->cima[2] = cimZ;

	float ux,uy,uz;
	float vx,vy,vz;
	float wx,wy,wz;

	wx = posX - focX; wy = posY - focY; wz = posZ - focZ;
	float wn = sqrtf(wx*wx + wy*wy + wz*wz);
	if(wn == 0) wn = 1.0f;
	wx /= wn; wy /= wn; wz /= wn;

	ux = camera->cima[1]*wz - camera->cima[2]*wy;
	uy = camera->cima[2]*wx - camera->cima[0]*wz;
	uz = camera->cima[0]*wy - camera->cima[1]*wx;
	float un = sqrtf(ux*ux + uy*uy + uz*uz);
	if(un == 0) un = 1.0f;
	ux /= un; uy /= un; uz /= un;

	vx = wy*uz - wz*uy;
	vy = wz*ux - wx*uz;
	vz = wx*uy - wy*ux;

	camera->viewMatrix[0][0] = ux; camera->viewMatrix[0][1] = uy; camera->viewMatrix[0][2] = uz;
	camera->viewMatrix[0][3] = -(ux*posX + uy*posY + uz*posZ);

	camera->viewMatrix[1][0] = vx; camera->viewMatrix[1][1] = vy; camera->viewMatrix[1][2] = vz;
	camera->viewMatrix[1][3] = -(vx*posX + vy*posY + vz*posZ);

	camera->viewMatrix[2][0] = wx; camera->viewMatrix[2][1] = wy; camera->viewMatrix[2][2] = wz;
	camera->viewMatrix[2][3] = -(wx*posX + wy*posY + wz*posZ);

	camera->viewMatrix[3][0] = 0.0f; camera->viewMatrix[3][1] = 0.0f; camera->viewMatrix[3][2] = 0.0f; camera->viewMatrix[3][3] = 1.0f;
}

//Desaloca a c�mera
void desalocaCamera(tCamera3d *objeto){
	if(!objeto) return;
	free(objeto->posicao);
	free(objeto->foco);
	free(objeto->cima);
	for(int i=0;i<4;i++) free(objeto->viewMatrix[i]);
	free(objeto->viewMatrix);
	free(objeto);
}
