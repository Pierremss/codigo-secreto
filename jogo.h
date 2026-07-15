#ifndef JOGO_H
#define JOGO_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TAM 51

// // Struct do arquivo de ranking
// typedef struct {
//     char nome[TAM];
//     int nivel;
//     int tentativas;
// } Ranking;

// Struct do jogo
typedef struct {
    char nomeJogador[TAM];
    int nivel;              // 1, 2 ou 3
    int tamanhoSequencia;   // 4, 5 ou 6
    int maxTentativas;      // 10, 12 ou 15
    int tentativasUsadas;   // Contador de rodadas
    int *sequenciaSecreta;  // Ponteiro para o vetor alocado
    int **historicoJogadas; // Ponteiro para a matriz alocada
} Jogo;

// Protótipos das funções de utilidade e memória
void limparBuffer(void);
void zeraVetorInt(int vetor[], int tamanho);
void zeraVetorChar(char vetor[], int tamanho);
int *alocaVetor(int i);
int **alocaMatriz(int i, int j);
void liberaMatriz(int **matriz, int i);

// Protótipos das funções do jogo
void inicializaJogo(Jogo *partidaAtual);
void geraSequenciaAleatoria(Jogo *partidaAtual);
void novoJogo(Jogo *partidaAtual);
void jogo(Jogo *partidaAtual);
void exibeMenuPrincipal();
void exibeAjuda();
void exibeRanking();
char* comparaCombinacao(int tentativa[], int secreta[], char dica[], int tamanho);
void definirDifuculdade(Jogo *partidaAtual);
void liberarJogo(Jogo *partidaAtual);

// Funções planejadas (protótipos para quando você for implementar)
// void carregaJogo(Jogo *jogo, char nomeArquivo[TAM]);
void salvaJogo(Jogo *jogo);
// void atualizaRanking(char nome[TAM], int nivel, int tentativas);
void exibeHistorico(Jogo *jog, char dicaAtual[]);

#endif