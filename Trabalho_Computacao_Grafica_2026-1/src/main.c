#include <stdio.h>
#include "objeto.h"
#include "viewport.h"

int main(void) {
    int **tela = criaTela(nColuna, nLinha);
    Objeto *obj = carregarArquivo("casaNDC.dcg");

    if (!obj) return 1;

    char comando;
    while(1) {
        limpaTela(tela);
        transformaObj(tela, obj);
        imprimeTela(tela);

        printf("\n[WASD] Move | [RE] Gira | [+-] Escala | [Q] Sai\nAcao: ");
        scanf(" %c", &comando);

        if (comando == 'q') break;

        switch(comando) {
            case 'w': obj->ty += 0.1; break;
            case 's': obj->ty -= 0.1; break;
            case 'a': obj->tx -= 0.1; break;
            case 'd': obj->tx += 0.1; break;
            case 'e': obj->theta += 0.1; break;
            case 'r': obj->theta -= 0.1; break;
            case '+': obj->sx += 0.1; obj->sy += 0.1; break;
            case '-': obj->sx -= 0.1; obj->sy -= 0.1; break;
        }
    }
    return 0;
}
