#include <stdio.h>
#include <stdlib.h>
#include "objeto.h"
#include "viewport.h"

#define NUM_OBJETOS 3

int main(void) {
    int **tela = criaTela(nColuna, nLinha);
    Objeto *objetos[NUM_OBJETOS];
    int selecionado = 0;

    objetos[0] = carregarArquivo("casaNDC.dcg");
    objetos[1] = carregarArquivo("estrelaNDC.dcg"); 
    objetos[2] = carregarArquivo("trianguloNDC.dcg");

    for (int i = 0; i < NUM_OBJETOS; i++) {
        if (!objetos[i]) {
            printf("Erro: Falha ao carregar o objeto %d. Verifique os arquivos .dcg\n", i + 1);
            return 1;
        }
    }

    char comando;
    int rodando = 1;

    while (rodando) {
        limpaTela(tela);

        for (int i = 0; i < NUM_OBJETOS; i++) {
            transformaObj(tela, objetos[i]);
        }

        imprimeTela(tela);

        printf("\n--- STATUS ---");
        printf("\nEditando Objeto: %d (Use 1, 2 ou 3 para trocar)", selecionado + 1);
        printf("\nComandos: [W,A,S,D] Move | [R,E] Gira | [+, -] Escala | [Q] Sair");
        printf("\nAcao: ");
        scanf(" %c", &comando);

        switch (comando) {
            case '1': selecionado = 0; break;
            case '2': selecionado = 1; break;
            case '3': selecionado = 2; break;

            case 'w': objetos[selecionado]->ty += 0.1; break;
            case 's': objetos[selecionado]->ty -= 0.1; break;
            case 'a': objetos[selecionado]->tx -= 0.1; break;
            case 'd': objetos[selecionado]->tx += 0.1; break;

            case 'e': objetos[selecionado]->theta += 0.1; break;
            case 'r': objetos[selecionado]->theta -= 0.1; break;

            case '+': 
                objetos[selecionado]->sx += 0.1; 
                objetos[selecionado]->sy += 0.1; 
                break;
            case '-': 
                objetos[selecionado]->sx -= 0.1; 
                objetos[selecionado]->sy -= 0.1; 
                break;

            case 'q': rodando = 0; break;
            
            default: printf("\nComando invalido!\n"); break;
        }
    }

    for (int i = 0; i < NUM_OBJETOS; i++) {
        desalocaObjeto(objetos[i]);
    }
    return 0;
}