#ifndef RANKING_H
#define RANKING_H

#include <stdio.h>
#include <stdlib.h>
#include "jogo.h"
#define TAM 51
#define TOP_RANKING 10

// Struct do arquivo de ranking
typedef struct {
    char nome[TAM];
    int nivel;
    int tentativas;
}Ranking;

int salvarRanking(Jogo jogoAtual);
void exibirRanking(void);

#endif