#include <stdio.h>
#include <SDL2/SDL.h>
#include "objeto.h"
#include "tela.h"
#include "algebra.h"
#include "camera.h"
#include <stdlib.h>
#include <math.h>

/*
* Controles:
*   TAB                 : alternar entre modo CAMERA e modo OBJETO
*
*   === Modo CAMERA ===
*   W / S               : mover camera para frente / tras (eixo Z)
*   A / D               : mover camera para esquerda / direita (eixo X)
*   R / F               : mover camera para cima / baixo (eixo Y)
*   Setas Cima/Baixo    : rotacionar camera em torno de X
*   Setas Esq/Dir       : rotacionar camera em torno de Y
*
*   === Modo OBJETO (cubo da esquerda - obj1) ===
*   W / S               : mover objeto no eixo Z
*   A / D               : mover objeto no eixo X
*   R / F               : mover objeto no eixo Y
*   Seta Cima/Baixo     : rotacionar objeto eixo X
*   Seta Esq/Dir        : rotacionar objeto eixo Y
*   Q / E               : diminuir / aumentar escala
*/

typedef enum {
    MODO_CAMERA,
    MODO_OBJETO 
} tModoControle;

void desenhaObjetoTela(SDL_Renderer *renderer, tCamera3d *cam, tObjeto3d *objeto){
    if(!objeto || !cam) return;

    const float focal = 1.0f;

    for(int i = 0; i < objeto->nArestas; i++){
        int a = objeto->arestas[i][0];
        int b = objeto->arestas[i][1];
        if(a < 0 || a >= objeto->nPontos || b < 0 || b >= objeto->nPontos) continue;

        float *pA_world = multMatriz4dPonto(objeto->modelMatrix, objeto->pontos[a]);
        float *pB_world = multMatriz4dPonto(objeto->modelMatrix, objeto->pontos[b]);

        float *pA_cam = multMatriz4dPonto(cam->viewMatrix, pA_world);
        float *pB_cam = multMatriz4dPonto(cam->viewMatrix, pB_world);

        if(pA_cam[2] >= 0 || pB_cam[2] >= 0){
            free(pA_world); free(pB_world); free(pA_cam); free(pB_cam);
            continue;
        }

        float ndcA_x = (focal * pA_cam[0]) / -pA_cam[2];
        float ndcA_y = (focal * pA_cam[1]) / -pA_cam[2];
        float ndcB_x = (focal * pB_cam[0]) / -pB_cam[2];
        float ndcB_y = (focal * pB_cam[1]) / -pB_cam[2];

        float vA[2] = { ndcA_x, ndcA_y };
        float vB[2] = { ndcB_x, ndcB_y };

        desenhaArestaTela(renderer, vA, vB);

        free(pA_world); free(pB_world); free(pA_cam); free(pB_cam);
    }
}

void atualizaTitulo(SDL_Window *window, tModoControle modo, int objSelecionado){
    char titulo[128];
    if(modo == MODO_CAMERA)
        SDL_snprintf(titulo, sizeof(titulo), "CG T2 | Modo: CAMERA (TAB para trocar)");
    else
        SDL_snprintf(titulo, sizeof(titulo), "CG T2 | Modo: OBJETO %d (TAB=modo, 1/2=cubo)", objSelecionado);
    SDL_SetWindowTitle(window, titulo);
}

int main( int argc, char * argv[] ){

    if (SDL_Init( SDL_INIT_EVERYTHING) < 0){
        printf("SDL nao inicializou! SDL Erro: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window = criaTela("CG T2");
    if(window == NULL){
        printf("SDL nao criou a janela! SDL Erro: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    tObjeto3d *obj1 = carregaObjeto("data/cubo.dcg");
    tObjeto3d *obj2 = carregaObjeto("data/cubo2.dcg");

    if(!obj1 || !obj2){
        printf("Erro ao carregar os objetos!\n");
        return EXIT_FAILURE;
    }

    transladaObjeto(obj1, -15.0f, 0.0f, 0.0f);
    transladaObjeto(obj2,  15.0f, 0.0f, 0.0f);

    tCamera3d *cam = criaCamera();
    float camX = 0.0f, camY = 0.0f, camZ = 60.0f;
    float camUpX = 0.0f, camUpY = 1.0f, camUpZ = 0.0f;
    defineCamera(cam, camX, camY, camZ,  0.0f, 0.0f, 0.0f,  camUpX, camUpY, camUpZ);

    const float transStep  = 2.0f;
    const float rotStep    = 5.0f;
    const float scaleStep  = 0.1f;
    const float camStep    = 3.0f;

    tModoControle modo = MODO_CAMERA;
    int objSelecionado = 1;
    atualizaTitulo(window, modo, objSelecionado);

    SDL_Event windowEvent;

    while(1){
        if( SDL_PollEvent(&windowEvent)){
            if(windowEvent.type == SDL_QUIT){
                break;
            }
            if(windowEvent.type == SDL_KEYDOWN){
                SDL_Keycode k = windowEvent.key.keysym.sym;

                if(k == SDLK_TAB){
                    modo = (modo == MODO_CAMERA) ? MODO_OBJETO : MODO_CAMERA;
                    atualizaTitulo(window, modo, objSelecionado);
                }
               
                if(k == SDLK_1){ objSelecionado = 1; atualizaTitulo(window, modo, objSelecionado); }
                if(k == SDLK_2){ objSelecionado = 2; atualizaTitulo(window, modo, objSelecionado); }

                if(modo == MODO_CAMERA){
                    if(k == SDLK_w){ camZ -= camStep; }
                    if(k == SDLK_s){ camZ += camStep; }
                    if(k == SDLK_a){ camX -= camStep; }
                    if(k == SDLK_d){ camX += camStep; }
                    if(k == SDLK_r){ camY += camStep; }
                    if(k == SDLK_f){ camY -= camStep; }

                    if(k == SDLK_UP    || k == SDLK_DOWN || k == SDLK_LEFT || k == SDLK_RIGHT){
                        float r;
                        float cx = camX, cy = camY, cz = camZ;
                        float dist = sqrtf(cx*cx + cy*cy + cz*cz);
                        if(dist == 0.0f) dist = 1.0f;

                        if(k == SDLK_LEFT){
                            r = rotStep * (float)M_PI / 180.0f;
                            float nx = cx * cosf(r) - cz * sinf(r);
                            float nz = cx * sinf(r) + cz * cosf(r);
                            float nUpX = camUpX * cosf(r) - camUpZ * sinf(r);
                            float nUpZ = camUpX * sinf(r) + camUpZ * cosf(r);
                            camX = nx; camZ = nz;
                            camUpX = nUpX; camUpZ = nUpZ;
                        }
                        if(k == SDLK_RIGHT){
                            r = -rotStep * (float)M_PI / 180.0f;
                            float nx = cx * cosf(r) - cz * sinf(r);
                            float nz = cx * sinf(r) + cz * cosf(r);
                            float nUpX = camUpX * cosf(r) - camUpZ * sinf(r);
                            float nUpZ = camUpX * sinf(r) + camUpZ * cosf(r);
                            camX = nx; camZ = nz;
                            camUpX = nUpX; camUpZ = nUpZ;
                        }
                        if(k == SDLK_UP){
                            r = rotStep * (float)M_PI / 180.0f;
                            float ny = cy * cosf(r) - cz * sinf(r);
                            float nz = cy * sinf(r) + cz * cosf(r);
                            float nUpY = camUpY * cosf(r) - camUpZ * sinf(r);
                            float nUpZ = camUpY * sinf(r) + camUpZ * cosf(r);
                            camY = ny; camZ = nz;
                            camUpY = nUpY; camUpZ = nUpZ;
                        }
                        if(k == SDLK_DOWN){
                            r = -rotStep * (float)M_PI / 180.0f;
                            float ny = cy * cosf(r) - cz * sinf(r);
                            float nz = cy * sinf(r) + cz * cosf(r);
                            float nUpY = camUpY * cosf(r) - camUpZ * sinf(r);
                            float nUpZ = camUpY * sinf(r) + camUpZ * cosf(r);
                            camY = ny; camZ = nz;
                            camUpY = nUpY; camUpZ = nUpZ;
                        }
                    }
                    defineCamera(cam, camX, camY, camZ,  0.0f, 0.0f, 0.0f,  camUpX, camUpY, camUpZ);

                } else {
                    tObjeto3d *obj = (objSelecionado == 1) ? obj1 : obj2;
                    switch(k){
                        case SDLK_w: transladaObjeto(obj, 0.0f, 0.0f, -transStep); break;
                        case SDLK_s: transladaObjeto(obj, 0.0f, 0.0f,  transStep); break;
                        case SDLK_a: transladaObjeto(obj, -transStep, 0.0f, 0.0f); break;
                        case SDLK_d: transladaObjeto(obj,  transStep, 0.0f, 0.0f); break;
                        case SDLK_r: transladaObjeto(obj, 0.0f,  transStep, 0.0f); break;
                        case SDLK_f: transladaObjeto(obj, 0.0f, -transStep, 0.0f); break;
                        case SDLK_UP:    rotacionaObjetoEixoX(obj, -rotStep); break;
                        case SDLK_DOWN:  rotacionaObjetoEixoX(obj,  rotStep); break;
                        case SDLK_LEFT:  rotacionaObjetoEixoY(obj, -rotStep); break;
                        case SDLK_RIGHT: rotacionaObjetoEixoY(obj,  rotStep); break;
                        case SDLK_q: escalaObjeto(obj, 1.0f - scaleStep, 1.0f - scaleStep, 1.0f - scaleStep); break;
                        case SDLK_e: escalaObjeto(obj, 1.0f + scaleStep, 1.0f + scaleStep, 1.0f + scaleStep); break;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 220, 50, 255);
        if(obj1) desenhaObjetoTela(renderer, cam, obj1);

        SDL_SetRenderDrawColor(renderer, 50, 200, 255, 255);
        if(obj2) desenhaObjetoTela(renderer, cam, obj2);

        SDL_RenderPresent(renderer);
    }

    desalocaTela(window);
    desalocaCamera(cam);
    desalocaObjeto(obj1);
    desalocaObjeto(obj2);
    SDL_Quit();

    return EXIT_SUCCESS;
}
