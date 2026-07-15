#include "jogo.h"
#include "ranking.h"

void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void zeraVetorInt(int vetor[], int tamanho){
    for (int i = 0; i < tamanho; i++)
        vetor[i] = 0;
}

void zeraVetorChar(char vetor[], int tamanho){
    for (int i = 0; i < tamanho; i++)
        vetor[i] = '\0';
}

void inicializaJogo(Jogo *partidaAtual){
    partidaAtual->nomeJogador[0] = '\0';
    partidaAtual->nivel = 0;
    partidaAtual->tamanhoSequencia = 0;
    partidaAtual->maxTentativas = 0;
    partidaAtual->tentativasUsadas = 0;
    partidaAtual->sequenciaSecreta = NULL;
    partidaAtual->historicoJogadas = NULL;
}

void exibeMenuPrincipal(){
    printf("\n==============================\nJOGO CODIGO SECRETO DE CORES\n==============================\n");
    printf("\nOpções de jogo:\n\n");
    printf("V - Voltar ao jogo em andamento\n");
    printf("A - Ajuda\n");
    printf("N - Novo Jogo\n");
    printf("C - Carregar Jogo\n");
    printf("S - Salvar Jogo\n");
    printf("R - Ranking\n");
    printf("X - Sair\n");
}

void exibeAjuda(){
    printf("\n==============================\nAJUDA\n==============================\n");
    printf("\nO jogo consiste em adivinhar uma sequência de cores.\n");
    printf("Cada rodada, você deve inserir uma sequência de cores.\n");
    printf("Após cada tentativa, o jogo informará quantas cores estão corretas e na posição correta.\n");
    printf("Você tem um número limitado de tentativas para adivinhar a sequência secreta.\n");
}

void exibeRanking(){
    printf("\n==============================\nRANKING\n==============================\n");
    // implementar a logica do ranking
}

// Salva a partida atual em texto no arquivo .cor escolhido pelo usuario.
void salvaJogo(Jogo *partidaAtual){
    // char nomeArquivo[TAM + 5];
    // char caminhoArquivo[TAM + 9];

    // printf("\nDigite o nome do arquivo: ");
    // scanf("%50s", nomeArquivo);

    // snprintf(caminhoArquivo, sizeof(caminhoArquivo), "%s.cor", nomeArquivo);

    char nomeArquivo[TAM + 5]; //mais cinco porque vai ter o .cor e o \0 no final da string

    printf("\nDigite o nome do arquivo (no maximo 50 caracteres): ");
    scanf("%50s", nomeArquivo);
    strcat(nomeArquivo, ".cor");

    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("\nErro ao abrir o arquivo para salvar o jogo.\n");
        return;
    }

    // Linha 1: nome do jogador.
    fprintf(arquivo, "%s\n", partidaAtual->nomeJogador);

    // Linha 2: nivel escolhido como letra.
    if (partidaAtual->nivel == 1)
        fprintf(arquivo, "F\n");
    else if (partidaAtual->nivel == 2)
        fprintf(arquivo, "M\n");
    else if (partidaAtual->nivel == 3)
        fprintf(arquivo, "D\n");
    else
        fprintf(arquivo, "?\n");

    // Linha 3: sequencia correta da senha secreta.
    for (int i = 0; i < partidaAtual->tamanhoSequencia; i++) {
        fprintf(arquivo, "%d", partidaAtual->sequenciaSecreta[i]);
        if (i < partidaAtual->tamanhoSequencia - 1)
            fprintf(arquivo, " ");
    }
    fprintf(arquivo, "\n");

    // Linha 4: quantidade de tentativas ja jogadas.
    fprintf(arquivo, "%d\n", partidaAtual->tentativasUsadas);

    // Linha 5 em diante: historico de tentativas, uma por linha.
    for (int i = 0; i < partidaAtual->tentativasUsadas; i++) {
        for (int j = 0; j < partidaAtual->tamanhoSequencia; j++) {
            fprintf(arquivo, "%d", partidaAtual->historicoJogadas[i][j]);
            if (j < partidaAtual->tamanhoSequencia - 1)
                fprintf(arquivo, " ");
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    printf("\nJogo salvo em %s\n", nomeArquivo);
}

//nessa funcao eu vou chamar o alocarVetor() e inicializar a sequencia secreta aleatoriamente dependendo do nivel
void geraSequenciaAleatoria(Jogo *partidaAtual){ 
    partidaAtual->sequenciaSecreta = alocaVetor(partidaAtual->tamanhoSequencia); //tamanho sequencia sera o tamanho do vetor (pode ser 4, 5 ou 6)

    //com isso vou preencher cada posicao da sequencia secreta com um numero de 1 a 6
    for (int i = 0; i < partidaAtual->tamanhoSequencia; i++){
        // partidaAtual->sequenciaSecreta[i] = (rand() % 6) + 1;
        partidaAtual->sequenciaSecreta[i] = 1;
    }
}

void novoJogo(Jogo *partidaAtual){
    inicializaJogo(partidaAtual);

    printf("\nDigite seu nome: ");
    scanf("%s", partidaAtual->nomeJogador);

    definirDifuculdade(partidaAtual);

    printf("Cores disponiveis:\n1 - Vermelho\n2 - Azul\n3 - Verde\n4 - Amarelo\n5 - Roxo\n6 - Laranja\n");

    geraSequenciaAleatoria(partidaAtual);

    partidaAtual->historicoJogadas = alocaMatriz(partidaAtual->maxTentativas, partidaAtual->tamanhoSequencia); //aloca a matriz de historico de jogadas

    //com tudo pronto, agora vou comecar a partida
    jogo(partidaAtual);
}
void jogo(Jogo *partidaAtual){
          
    int tentativaAtual[partidaAtual->tamanhoSequencia];
    char dicaAtual[partidaAtual->tamanhoSequencia + 1];

    //exibe o historico cseja uma partida em andamento, ou seja, se o jogador ja tiver feito alguma tentativa
    if(partidaAtual->tentativasUsadas > 0)
            exibeHistorico(partidaAtual, dicaAtual);

    while(partidaAtual->tentativasUsadas < partidaAtual->maxTentativas){
        // int tentativaAtual[partidaAtual->tamanhoSequencia];
        // char dicaAtual[partidaAtual->tamanhoSequencia + 1]; //+1 para o caractere nulo no final da string
        zeraVetorInt(tentativaAtual, partidaAtual->tamanhoSequencia);
        zeraVetorChar(dicaAtual, partidaAtual->tamanhoSequencia + 1 );

        printf("\nTentativa %d de %d", partidaAtual->tentativasUsadas + 1, partidaAtual->maxTentativas);
        printf("\nDigite %d cores (0 para voltar ao menu):", partidaAtual->tamanhoSequencia);
       
        //lendo a tentativa e validando se o jogador quer voltar ao menu
        for(int i = 0; i < partidaAtual->tamanhoSequencia; i++){
            scanf("%d", &tentativaAtual[i]);
            if(tentativaAtual[i] == 0){
                printf("\nVoltando ao menu principal...\n");
                return;
            }
        }
        //salvando no historico de jogadas
        for(int i = 0; i < partidaAtual->tamanhoSequencia; i++)
            partidaAtual->historicoJogadas[partidaAtual->tentativasUsadas][i] = tentativaAtual[i];

        partidaAtual->tentativasUsadas++; // incrementando o contador de tentativas usadas
        // exibeHistorico(partidaAtual, dicaAtual);

        exibeHistorico(partidaAtual, dicaAtual);

         //verificando se o jogador ganhou
            int win = 0;
            for (int i = 0; i < partidaAtual->tamanhoSequencia; i++){
                if (dicaAtual[i] == 'C')
                    win++;
            }
            if(win == partidaAtual->tamanhoSequencia){
                printf("\n\nParabéns! Você acertou a sequência secreta!\n");
                salvarRanking(*partidaAtual);
                limparBuffer();
                getchar();
                return;
            }

            printf("\n");
        }
    }


void exibeHistorico(Jogo *partidaAtual, char dicaAtual[]){

    printf("\nResultado:\n");
        for (int i = 0; i < partidaAtual->tentativasUsadas; i++) {
            printf("Rodada %d:  ", i+1);
            for (int j = 0; j < partidaAtual->tamanhoSequencia; j++) {
                printf("%d ", partidaAtual->historicoJogadas[i][j]);
            }
        
            //dica de cada rodada, comparando a tentativa com a sequencia secreta
            comparaCombinacao(partidaAtual->historicoJogadas[i], partidaAtual->sequenciaSecreta, dicaAtual, partidaAtual->tamanhoSequencia);
            printf("%s", dicaAtual);
        printf("\n");
}
}

//essa funcao devera comparar a tentativa do jogador com a senha e retornar um vetor dica. Nesse vetor primeiro virah primeiro o C, depois E, depois -. (ex: [C C E - -])
char* comparaCombinacao(int tentativa[], int secreta[], char dica[], int tamanho){
    int usadaTentativa[tamanho];
    int usadaSecreta[tamanho];
    zeraVetorInt(usadaTentativa, tamanho);
    zeraVetorInt(usadaSecreta, tamanho);
    int indice = 0;

    for (int i = 0; i < tamanho; i++) {
        if (tentativa[i] == secreta[i]) {
            dica[indice++] = 'C';
            usadaTentativa[i] = 1;
            usadaSecreta[i] = 1;
        }
    }

    for (int i = 0; i < tamanho; i++) {
        if (usadaTentativa[i])
            continue;

        for (int j = 0; j < tamanho; j++) {
            if (usadaSecreta[j]) 
                continue;

            if (tentativa[i] == secreta[j]) {
                dica[indice++] = 'E';
                usadaTentativa[i] = 1;
                usadaSecreta[j] = 1;
                break;
            }
        }
    }

    while (indice < tamanho) {
        dica[indice++] = '-';
    }
    dica[indice] = '\0';
    return dica;
}

void definirDifuculdade(Jogo *partidaAtual){
    int nivel;
    do{
        printf("\nEscolha o nível de dificuldade:\n");
        printf("1 - Fácil (4 cores, 10 tentativas)\n");
        printf("2 - Médio (5 cores, 12 tentativas)\n");
        printf("3 - Difícil (6 cores, 15 tentativas)\n");
        printf("Digite o nível desejado: ");
        scanf("%d", &nivel);

        switch (nivel){
            case 1:
                partidaAtual->nivel = 1;
                partidaAtual->tamanhoSequencia = 4;
                partidaAtual->maxTentativas = 10;
                break;
            case 2:
                partidaAtual->nivel = 2;
                partidaAtual->tamanhoSequencia = 5;
                partidaAtual->maxTentativas = 12;
                break;
            case 3:
                partidaAtual->nivel = 3;
                partidaAtual->tamanhoSequencia = 6;
                partidaAtual->maxTentativas = 15;
                break;
            default:
                printf("\nNível inválido. Digite novamente.\n");
        }
    }while(nivel < 1 || nivel > 3);
}

int *alocaVetor(int i){
    if (i <= 0)
        return NULL;
    else{
        int *vetor = NULL;
        vetor = malloc(i * sizeof(int));
        return vetor;
    }
}

int **alocaMatriz(int i, int j){
    if (i <= 0 || j <= 0)
        return NULL;
    else{
        int **matriz = NULL;
        matriz = malloc(i * sizeof(int*));
        for (int k = 0; k < i; k++)
            matriz[k] = malloc(j * sizeof(int));
        return matriz;
    }
}

void liberaMatriz(int **matriz, int i){
    if(matriz == NULL)
        return;

    for (int a = 0; a < i; a++)
        if(matriz[a] != NULL)
            free(matriz[a]);
        else
            continue;

    free(matriz);   
}

//liberar a memoria alocada para o jogo, incluindo a matriz de historico de jogadas e o vetor da sequencia secreta
void liberarJogo(Jogo *partidaAtual){
    if(partidaAtual->historicoJogadas != NULL)
        liberaMatriz(partidaAtual->historicoJogadas, partidaAtual->maxTentativas);
    if(partidaAtual->sequenciaSecreta != NULL)
        free(partidaAtual->sequenciaSecreta);
}

