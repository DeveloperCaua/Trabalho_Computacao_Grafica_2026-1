# Trabalho de Computação Gráfica (2026/1)

## Descrição

Repositório com as implementações realizadas para a entrega 01 da disciplina de Computação Gráfica (Semestre 2026/1). O projeto contém código em C que carrega e renderiza objetos simples definidos no formato `.dcg` localizado na pasta `data/`.

## Estado do projeto
- Linguagem principal: C
- Estrutura simples para estudo de rasterização, transformações e viewport

## Estrutura do repositório

- [data/casaNDC.dcg](data/casaNDC.dcg) — modelo de exemplo (casa)
- [data/estrelaNDC.dcg](data/estrelaNDC.dcg) — modelo de exemplo (estrela)
- [data/trianguloNDC.dcg](data/trianguloNDC.dcg) — modelo de exemplo (triângulo)
- [include/objeto.h](include/objeto.h)
- [include/viewport.h](include/viewport.h)
- [src/main.c](src/main.c)
- [src/objeto.c](src/objeto.c)
- [src/viewport.c](src/viewport.c)

## Requisitos

- Compilador C (GCC/Clang) ou ambiente compatível (MinGW no Windows, GCC no Linux/macOS)

## Como compilar

Instruções  para compilar com :

Existe um `Makefile` pronto dentro da pasta do exercício. Para usá-lo, abra um terminal na raiz do projeto onde estão as pastas `src/` e `include/`. No layout atual do repositório o `Makefile` está na raiz do projeto — siga os comandos abaixo a partir da pasta que contém esse `Makefile`:

```sh
# entrar na pasta onde está o Makefile
cd Trabalho_Computacao_Grafica_2026-1

# compilar (gera bin/trabalho_cg)
make

# executar com um exemplo (usa data/casaNDC.dcg)
make run

# limpar artefatos
make clean
```

No Windows, se o comando `make` não estiver disponível, use `mingw32-make` (MinGW) ou instale o `make` via MSYS2/Chocolatey.

## Como executar

Após compilar, execute o binário passando um arquivo `.dcg` como argumento (se o `main.c` aceitar argumento de arquivo). Exemplos:

```sh
# Linux/macOS
./bin/trabalho_cg data/casaNDC.dcg

# Windows
bin\trabalho_cg.exe data\casaNDC.dcg
```

Se o programa não aceitar argumento, consulte o código em [src/main.c](src/main.c) para instruções sobre como fornecer entradas.

## Formato dos arquivos de dados

Os arquivos em `data/` têm extensão `.dcg` e descrevem objetos no espaço NDC (Normalized Device Coordinates). Consulte `src/objeto.c` e `include/objeto.h` para detalhes da implementação e do formato esperado.

## Testes e validação

Este repositório não contém um framework de testes automatizados. Para validar o funcionamento:

- Compile o projeto conforme descrito acima
- Execute o binário com os arquivos em `data/`
- Verifique a saída (impressões no console ou arquivos gerados) conforme o comportamento implementado

## Boas práticas e dicas

- Use `-g` no `gcc` para gerar símbolos de debug se for necessário depurar:

```sh
gcc -std=c11 -Wall -Wextra -g src/*.c -I include -o bin/trabalho_cg
```

- Ative `-fsanitize=address,undefined` em compiladores compatíveis para encontrar erros de memória durante desenvolvimento.