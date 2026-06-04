#include <stdio.h>
#include <SDL2/SDL.h>
#include "objeto.h"
#include "tela.h"
#include "algebra.h"
#include "camera.h"
#include <stdlib.h>
#include <math.h>

//desenha um objeto na tela usando projeção em perspectiva
void desenhaObjetoTela(SDL_Renderer *renderer, tCamera3d *cam, tObjeto3d *objeto){
    if(!objeto || !cam) return;

    const float focal = 1.0f; // distância focal (ajuste se necessário)

    for(int i = 0; i < objeto->nArestas; i++){
        int a = objeto->arestas[i][0];
        int b = objeto->arestas[i][1];
        if(a < 0 || a >= objeto->nPontos || b < 0 || b >= objeto->nPontos) continue;

        float *pA_world = multMatriz4dPonto(objeto->modelMatrix, objeto->pontos[a]);
        float *pB_world = multMatriz4dPonto(objeto->modelMatrix, objeto->pontos[b]);

        float *pA_cam = multMatriz4dPonto(cam->viewMatrix, pA_world);
        float *pB_cam = multMatriz4dPonto(cam->viewMatrix, pB_world);

        // rejeita pontos atrás da câmera
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


int main( int argc, char * argv[] ){

    if (SDL_Init( SDL_INIT_EVERYTHING) < 0){
        printf("SDL n�o inicializou! SDL Erro: %s\n", SDL_GetError());
    }

    SDL_Window *window = criaTela("Hello SDL World!");

    if(window == NULL){
        printf("SDL n�o criou a janela! SDL Erro: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    // exemplo: carregar objeto e criar camera
    tObjeto3d *obj = carregaObjeto("data/cubo.dcg");
    tCamera3d *cam = criaCamera();
    defineCamera(cam, 0.0f, 0.0f, 30.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f);

    // velocidade / passo de transformação
    const float transStep = 1.0f;
    const float rotStep = 5.0f; // graus
    const float scaleStep = 0.1f;

    SDL_Event windowEvent;

    while(1){
        if( SDL_PollEvent(&windowEvent)){
            if(windowEvent.type == SDL_QUIT){
                break;
            }
            if(windowEvent.type == SDL_KEYDOWN){
                SDL_Keycode k = windowEvent.key.keysym.sym;
                switch(k){
                    case SDLK_w: transladaObjeto(obj, 0.0f, 0.0f, -transStep); break;
                    case SDLK_s: transladaObjeto(obj, 0.0f, 0.0f, transStep); break;
                    case SDLK_a: transladaObjeto(obj, -transStep, 0.0f, 0.0f); break;
                    case SDLK_d: transladaObjeto(obj, transStep, 0.0f, 0.0f); break;
                    case SDLK_UP: rotacionaObjetoEixoX(obj, -rotStep); break;
                    case SDLK_DOWN: rotacionaObjetoEixoX(obj, rotStep); break;
                    case SDLK_LEFT: rotacionaObjetoEixoY(obj, -rotStep); break;
                    case SDLK_RIGHT: rotacionaObjetoEixoY(obj, rotStep); break;
                    case SDLK_q: escalaObjeto(obj, 1.0f - scaleStep, 1.0f - scaleStep, 1.0f - scaleStep); break;
                    case SDLK_e: escalaObjeto(obj, 1.0f + scaleStep, 1.0f + scaleStep, 1.0f + scaleStep); break;
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        // desenha objeto de exemplo em perspectiva
        if(obj) desenhaObjetoTela(renderer, cam, obj);

        SDL_RenderPresent(renderer);
    }

    desalocaTela(window);
    desalocaCamera(cam);
    desalocaObjeto(obj);
    SDL_Quit();

    return EXIT_SUCCESS;
}
