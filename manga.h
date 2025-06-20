#ifndef MANGA_H
#define MANGA_H

#define MAX_VOLUMES 100
#define MAX_MANGAS 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char isbn[20];
    char titulo[100];
    char autores[100];
    int anoInicio;
    int anoFim; // -1 se indefinido
    char genero[100];
    char revista[100];
    char editora[100];
    int anoEdicao;
    int qtdVolumes;
    int qtdAdquiridos;
    int volumes[MAX_VOLUMES];
} Manga;

typedef struct {
    char isbn[20];
    long posicao; // posição no arquivo
} IndicePrimario;

typedef struct {
    char titulo[100];
    char isbn[20];
} IndiceSecundario;

void carregarRegistros();
void salvarRegistro(Manga *m);
void criarRegistro();
void lerRegistro();
void atualizarRegistro();
void apagarRegistro();
void salvarIndices();
void carregarIndices();
void menu();
void criarRegistroPorLinha();
void criarRegistrosDeArquivo();

#endif