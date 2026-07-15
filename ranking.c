#include "ranking.h"
#include "jogo.h"

// Compara dois registros para ordenar primeiro por tentativas, depois por nivel e depois por nome.
static int compararRanking(Ranking a, Ranking b){
    if (a.tentativas != b.tentativas)
        return a.tentativas < b.tentativas;

    if (a.nivel != b.nivel)
        return a.nivel > b.nivel;

    return strcmp(a.nome, b.nome) < 0;
}

// Insere um novo registro no vetor na posicao correta, mantendo a ordem do ranking.
static void inserirOrdenado(Ranking ranking[], int *quantidade, Ranking novoRanking){
    int posicao = *quantidade;

    // Desloca os elementos menores para abrir espaco para o novo registro.
    while (posicao > 0 && compararRanking(novoRanking, ranking[posicao - 1])) {
        if (posicao < TOP_RANKING)
            ranking[posicao] = ranking[posicao - 1];
        posicao--;
    }

    // Grava o novo registro apenas se ele ainda estiver dentro do Top 10.
    if (posicao < TOP_RANKING) {
        ranking[posicao] = novoRanking;
        if (*quantidade < TOP_RANKING)
            (*quantidade)++;
    }
}

// Le todos os registros salvos no arquivo binario e monta o ranking em memoria.
static int carregarRanking(Ranking ranking[]){
    FILE *arquivo = fopen("ranking.bin", "rb");
    if (arquivo == NULL)
        return 0;

    Ranking atual;
    int quantidade = 0;

    while (fread(&atual, sizeof(Ranking), 1, arquivo) == 1)
        inserirOrdenado(ranking, &quantidade, atual);

    fclose(arquivo);
    return quantidade;
}

// Regrava o arquivo binario com o ranking ja ordenado.
static void salvarRankingOrdenado(Ranking ranking[], int quantidade){
    FILE *arquivo = fopen("ranking.bin", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de ranking.\n");
        return;
    }

    fwrite(ranking, sizeof(Ranking), quantidade, arquivo);
    fclose(arquivo);
}

// Verifica se o novo jogador merece entrar no Top 10 quando o ranking ja esta cheio.
static int entraNoRanking(Ranking novoRanking, Ranking ultimoRanking){
    if (novoRanking.tentativas < ultimoRanking.tentativas)
        return 1;

    if (novoRanking.tentativas > ultimoRanking.tentativas)
        return 0;

    if (novoRanking.nivel > ultimoRanking.nivel)
        return 1;

    if (novoRanking.nivel < ultimoRanking.nivel)
        return 0;

    return strcmp(novoRanking.nome, ultimoRanking.nome) < 0;
}

// Salva o resultado da partida, insere no ranking se necessario e grava tudo ordenado.
int salvarRanking(Jogo jogoAtual){
    Ranking ranking[TOP_RANKING];
    int quantidade = carregarRanking(ranking);
    Ranking novoRanking;

    // Copia os dados da partida para a estrutura que sera salva no ranking.
    strcpy(novoRanking.nome, jogoAtual.nomeJogador);
    novoRanking.nivel = jogoAtual.nivel;
    novoRanking.tentativas = jogoAtual.tentativasUsadas;

    // Se o ranking estiver cheio, so continua se o novo resultado for melhor que o ultimo.
    if (quantidade == TOP_RANKING && !entraNoRanking(novoRanking, ranking[TOP_RANKING - 1]))
        return 0;

    // Insere o novo registro na posicao correta.
    inserirOrdenado(ranking, &quantidade, novoRanking);

    // Garante que o arquivo nunca passe de 10 registros.
    if (quantidade > TOP_RANKING)
        quantidade = TOP_RANKING;

    // Grava o ranking final ja ordenado no arquivo binario.
    salvarRankingOrdenado(ranking, quantidade);
    return 0;
}

// Abre o arquivo binario e mostra o ranking salvo na ordem em que foi gravado.
void exibirRanking(void){
    FILE *arquivo = fopen("ranking.bin", "rb");
    if (arquivo == NULL) {
        printf("\nNenhum ranking encontrado.\n");
        return;
    }

    Ranking ranking;
    int posicao = 1;
    int registrosEncontrados = 0;

    printf("\n==============================\nRANKING\n==============================\n");

    // Lê cada registro do arquivo e imprime na tela.
    while (fread(&ranking, sizeof(Ranking), 1, arquivo) == 1) {
        printf("%d. Jogador: %s | Nível: %d | Tentativas: %d\n",
               posicao,
               ranking.nome,
               ranking.nivel,
               ranking.tentativas);
        posicao++;
        registrosEncontrados = 1;
    }

    // Caso o arquivo exista, mas nao tenha dados validos.
    if (!registrosEncontrados) {
        printf("Nenhum ranking encontrado.\n");
    }

    fclose(arquivo);
}