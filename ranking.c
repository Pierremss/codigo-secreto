// Nome: Pierre Miguel Silveira Silva Franco
// Matrícula: 26.1.4061

#include "ranking.h"
#include "jogo.h"

//compara dois registros para ordenar primeiro por tentativas, depois por nivel e depois por nome.
int compararRanking(Ranking a, Ranking b){
    if (a.tentativas != b.tentativas)
        return a.tentativas < b.tentativas; //se a quantidade de tentativas for menor, o registro a vem antes do registro b

    if (a.nivel != b.nivel)
        return a.nivel > b.nivel; //se o nivel for maior, o registro a vem antes do registro b
        
    return 0; //se for tudo igual, mantem a ordem original (nao troca)
}

//innsere um novo registro no vetor na posicao correta, mantendo a ordem do ranking
void inserirOrdenado(Ranking ranking[], int *quantidade, Ranking novoRanking){
    int posicao = *quantidade;

    //desloca os elementos menores para abrir espaco para o novo registro
    while (posicao > 0 && compararRanking(novoRanking, ranking[posicao - 1])) {
        if (posicao < TOP_RANKING) //garante que nao vai acessar posicao fora do vetor
            ranking[posicao] = ranking[posicao - 1]; //desloca o registro para a proxima posicao
        posicao--;
    }

    //grava o novo registro apenas se ele ainda estiver dentro do Top 10
    if (posicao < TOP_RANKING) {
        ranking[posicao] = novoRanking;
        if (*quantidade < TOP_RANKING) //se ainda nao atingiu o limite de 10 registros, incrementa a quantidade
            (*quantidade)++;
    }
}

//le todos os registros salvos no arquivo binario e monta o ranking em memoria
int carregarRanking(Ranking ranking[]){
    FILE *arquivo = fopen("ranking.rnk", "rb");
    if (arquivo == NULL)
        return 0;

    Ranking atual;
    int quantidade = 0;

    while (fread(&atual, sizeof(Ranking), 1, arquivo) == 1)
        inserirOrdenado(ranking, &quantidade, atual); //ordenando so de precaocao, caso o arquivo binario por algum motivo esteja desordenado

    fclose(arquivo);
    return quantidade;
}

//regrava o arquivo binario com o ranking ja ordenado
void salvarRankingOrdenado(Ranking ranking[], int quantidade){
    FILE *arquivo = fopen("ranking.rnk", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de ranking.\n");
        return;
    }

    fwrite(ranking, sizeof(Ranking), quantidade, arquivo);
    fclose(arquivo);
}

//verifica se o novo jogador merece entrar no Top 10 quando o ranking ja esta cheio
int entraNoRanking(Ranking novoRanking, Ranking ultimoRanking){
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

//salva o resultado da partida, insere no ranking se necessario e grava tudo ordenado
int salvarRanking(Jogo jogoAtual){
    Ranking ranking[TOP_RANKING];
    int quantidade = carregarRanking(ranking); 
    Ranking novoRanking;

    //copia os dados da partida para a estrutura que sera salva no ranking
    strcpy(novoRanking.nome, jogoAtual.nomeJogador);
    novoRanking.nivel = jogoAtual.nivel;
    novoRanking.tentativas = jogoAtual.tentativasUsadas;

    //se o ranking estiver cheio, so continua se o novo resultado for melhor que o ultimo
    if (quantidade == TOP_RANKING && !entraNoRanking(novoRanking, ranking[TOP_RANKING - 1]))
        return 1; //se o jogador nao entrou no ranking, retorna 1 para informar que nao foi adicionado

    //insere o novo registro na posicao correta
    inserirOrdenado(ranking, &quantidade, novoRanking);

    //garante que o arquivo nunca passe de 10 registros
    if (quantidade > TOP_RANKING)
        quantidade = TOP_RANKING;

    //grava o ranking final ja ordenado no arquivo binario
    salvarRankingOrdenado(ranking, quantidade);
    return 0;
}

//abre o arquivo binario e mostra o ranking salvo na ordem em que foi gravado
void exibirRanking(void){
    FILE *arquivo = fopen("ranking.rnk", "rb");
    if (arquivo == NULL) {
        printf("\nNenhum ranking encontrado.\n");
        return;
    }

    Ranking ranking;
    int posicao = 1;
    int registrosEncontrados = 0;

    printf("\n==============================\nRANKING\n==============================\n");

    //le cada registro do arquivo e imprime na tela
    while (fread(&ranking, sizeof(Ranking), 1, arquivo) == 1) {
        printf("%d. Jogador: %s | Nível: %d | Tentativas: %d\n",
               posicao,
               ranking.nome,
               ranking.nivel,
               ranking.tentativas);
        posicao++;
        registrosEncontrados = 1;
    }

    //caso o arquivo exista, mas nao tenha dados validos
    if (!registrosEncontrados) {
        printf("Nenhum ranking encontrado.\n");
    }

    printf("\nAperte Enter para voltar ao menu principal...\n");
    getchar();
    fclose(arquivo);
}