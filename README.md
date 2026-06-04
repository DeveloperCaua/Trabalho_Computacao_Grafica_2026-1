# Trabalho_Computacao_Grafica_2026-1
Implementações práticas e algoritmos desenvolvidos para a disciplina de Computação Gráfica (2026/1).

<div align="center">
  <img src="imgs/diagrama.png" width="500px" />
</div>

## Como compilar e executar (Windows)

Pré-requisitos:
- `gcc` (MinGW) ou `clang`
- SDL2 instalado e com include/libs disponíveis

Exemplo de compilação com MinGW (ajuste paths do SDL2 conforme seu sistema):

```bash
gcc -I"C:/SDL2/include" src/*.c -L"C:/SDL2/lib" -lmingw32 -lSDL2main -lSDL2 -o trabalho2.exe
```

Executar:

```bash
./trabalho2.exe
```

Coloque os arquivos de objetos em `data/` (ex.: `data/cubo.dcg`).

Observação para PowerShell/Windows:

- Certifique-se de executar o comando **a partir da raiz do projeto** (onde existe a pasta `src`). Se você executar o comando a partir de `C:\Windows\System32` (ou outra pasta que não contenha `src`), o GCC não encontrará os arquivos.