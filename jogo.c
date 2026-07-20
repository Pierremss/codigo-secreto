// Nome: Pierre Miguel Silveira Silva Franco
// Matrícula: 26.1.4061

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

void exibeAjuda() {
    printf("\n=======================================================\n");
    printf("                AJUDA - CÓDIGO SECRETO                   \n");
    printf("=======================================================\n\n");
    
    printf("OBJETIVO:\n");
    printf("O programa gera uma sequência secreta de cores e você deve tentar descobri-la.\n\n");
    
    printf("CORES DISPONÍVEIS:\n");
    printf("Você deve inserir sua tentativa usando os seguintes códigos numéricos:\n");
    printf("1 - Vermelho | 2 - Azul | 3 - Verde | 4 - Amarelo | 5 - Roxo | 6 - Laranja.\n\n");
    
    printf("NÍVEIS DE DIFICULDADE:\n");
    printf(" Fácil: Sequência de 4 cores e limite de 10 tentativas.\n");
    printf(" Médio: Sequência de 5 cores e limite de 12 tentativas.\n");
    printf(" Difícil: Sequência de 6 cores e limite de 15 tentativas.\n\n");
    
    printf("COMO FUNCIONAM AS PISTAS:\n");
    printf("Após cada tentativa válida, o programa informará o resultado da sua jogada usando símbolos:\n");
    printf(" [C] (Correta): A cor está correta e na posição correta.\n");
    printf(" [E] (Errada): A cor existe na sequência secreta, mas está em outra posição.\n");
    printf(" [-] (Inexistente): A cor não existe na sequência secreta.\n");
    printf("ATENÇÃO: A resposta com as dicas NÃO será exibida na mesma ordem em que você digitou as cores!\n\n");
    
    printf("CONTROLES ADICIONAIS:\n");
    printf(" Durante o jogo, se desejar desistir da partida atual e voltar ao menu principal, basta digitar 0.\n\n");
    
    printf("=======================================================\n");
}

int nivelDaLetra(char letra){
    if (letra == 'F' || letra == 'f')
        return 1;

    if (letra == 'M' || letra == 'm')
        return 2;

    if (letra == 'D' || letra == 'd')
        return 3;

    return 0;
}

// Salva a partida atual em texto no arquivo .cor escolhido pelo usuario.
void salvaJogo(Jogo *partidaAtual){
    char nomeArquivo[TAM + 5]; //mais cinco porque vai ter o .cor e o \0 no final da string

    printf("\nDigite o nome do arquivo (no maximo 50 caracteres): ");
    scanf("%50s", nomeArquivo);
    strcat(nomeArquivo, ".cor");

    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("\nErro ao abrir o arquivo para salvar o jogo.\n");
        return;
    }

    //linha 1 nome do jogador
    fprintf(arquivo, "%s\n", partidaAtual->nomeJogador);

    //linha 2 nivel escolhido como letra
    if (partidaAtual->nivel == 1)
        fprintf(arquivo, "F\n");
    else if (partidaAtual->nivel == 2)
        fprintf(arquivo, "M\n");
    else if (partidaAtual->nivel == 3)
        fprintf(arquivo, "D\n");
    else
        fprintf(arquivo, "@\n"); //grava um caracetere invalido para indicar que o arquivo nao tem um nivel valido

    //linha 3 sequencia correta da senha secreta
    for (int i = 0; i < partidaAtual->tamanhoSequencia; i++) {
        fprintf(arquivo, "%d", partidaAtual->sequenciaSecreta[i]);
        if (i < partidaAtual->tamanhoSequencia - 1)
            fprintf(arquivo, " ");
    }
    fprintf(arquivo, "\n");

    //linha 4 quantidade de tentativas ja jogadas
    fprintf(arquivo, "%d\n", partidaAtual->tentativasUsadas);

    //linha 5 em diante, historico de tentativas, uma por linha
    for (int i = 0; i < partidaAtual->tentativasUsadas; i++) {
        for (int j = 0; j < partidaAtual->tamanhoSequencia; j++) {
            fprintf(arquivo, "%d", partidaAtual->historicoJogadas[i][j]);
            if (j < partidaAtual->tamanhoSequencia - 1) //para nao colocar espaco no final da linha
                fprintf(arquivo, " ");
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    printf("\nJogo salvo em %s\n", nomeArquivo);
}

//carrega uma partida salva em .cor e reconstrui o estado atual do jogo
int carregaJogo(Jogo *partidaAtual){
    Jogo jogoCarregado;

    char nomeArquivo[TAM + 5]; //mais cinco porque vai ter o .cor e o \0 no final da string

    printf("\nDigite o nome do arquivo (no maximo 50 caracteres): ");
    scanf("%50s", nomeArquivo);
    strcat(nomeArquivo, ".cor");

    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("\nErro ao abrir o arquivo para carregar o jogo.\n");
        printf("\nAperte Enter para voltar ao menu principal...\n");
        limparBuffer();
        getchar();
        return 1;
    }

    inicializaJogo(&jogoCarregado); //inicializa a estrutura do jogo carregado para evitar lixo de memória

    if (fgets(jogoCarregado.nomeJogador, TAM, arquivo) == NULL) { //verifica se existe um nome de jogador no arquivo
        printf("\nArquivo invalido.\n");
        fclose(arquivo);
        return 1;
    }
    jogoCarregado.nomeJogador[strcspn(jogoCarregado.nomeJogador, "\n")] = '\0'; //se tiver, remove o \n do final do nome do jogador

    char nivelChar;
    if (fscanf(arquivo, " %c", &nivelChar) != 1) { //verifica se existe um nivel no arquivo, ou seja, se o scanf conseguiu ler um caractere
        printf("\nArquivo invalido.\n");
        fclose(arquivo);
        return 1;
    }

    jogoCarregado.nivel = nivelDaLetra(nivelChar); 
    if (jogoCarregado.nivel == 0) {
        printf("\nNivel invalido no arquivo.\n");
        fclose(arquivo);
        return 1;
    }

    if (jogoCarregado.nivel == 1) {
        jogoCarregado.tamanhoSequencia = 4;
        jogoCarregado.maxTentativas = 10;
    } else if (jogoCarregado.nivel == 2) {
        jogoCarregado.tamanhoSequencia = 5;
        jogoCarregado.maxTentativas = 12;
    } else {
        jogoCarregado.tamanhoSequencia = 6;
        jogoCarregado.maxTentativas = 15;
    }

    jogoCarregado.sequenciaSecreta = alocaVetor(jogoCarregado.tamanhoSequencia);
    if (jogoCarregado.sequenciaSecreta == NULL) { //menssagem de erro se nao conseguir alocar memoria para a sequencia secreta
        printf("\nErro de memoria ao carregar o jogo.\n");
        fclose(arquivo);
        return 1;
    }

    for (int i = 0; i < jogoCarregado.tamanhoSequencia; i++) { //bloco para ler a sequencia secreta
        if (fscanf(arquivo, "%d", &jogoCarregado.sequenciaSecreta[i]) != 1) { //se nao conseguir ler um numero, o arquivo esta invalido
            printf("\nArquivo invalido.\n");
            free(jogoCarregado.sequenciaSecreta);
            fclose(arquivo);
            return 1;
        }
    }

    if (fscanf(arquivo, "%d", &jogoCarregado.tentativasUsadas) != 1) { //verifica se existe a quantidade de tentativas ja jogadas no arquivo
        printf("\nArquivo invalido.\n");
        free(jogoCarregado.sequenciaSecreta);
        fclose(arquivo);
        return 1;
    }

    if (jogoCarregado.tentativasUsadas > jogoCarregado.maxTentativas) //verifica se a quantidade de tentativas ja jogadas no arquivo e maior que o maximo permitido para o nivel escolhido
        jogoCarregado.tentativasUsadas = jogoCarregado.maxTentativas;

    jogoCarregado.historicoJogadas = alocaMatriz(jogoCarregado.maxTentativas, jogoCarregado.tamanhoSequencia); //aloca a matriz de historico de jogadas com o tamanho maximo de tentativas e o tamanho da sequencia secreta
    if (jogoCarregado.historicoJogadas == NULL) {
        printf("\nErro de memoria ao carregar o jogo.\n");
        free(jogoCarregado.sequenciaSecreta);
        fclose(arquivo);
        return 1;
    }

    for (int i = 0; i < jogoCarregado.tentativasUsadas; i++) { //bloco para ler o historico de tentativas ja jogadas, uma por linha
        for (int j = 0; j < jogoCarregado.tamanhoSequencia; j++) {
            if (fscanf(arquivo, "%d", &jogoCarregado.historicoJogadas[i][j]) != 1) { //se nao conseguir ler um numero, o arquivo esta invalido
                printf("\nArquivo invalido.\n");
                liberaMatriz(jogoCarregado.historicoJogadas, jogoCarregado.maxTentativas); //libera a matriz de historico de jogadas alocada
                free(jogoCarregado.sequenciaSecreta); // libera o vetor de sequencia secreta alocado
                fclose(arquivo);
                return 1;
            }
        }
    }

    fclose(arquivo);

    liberarJogo(partidaAtual); //libera a memoria caso ja tenha um jogo em andamento antes de carregar o novo jogo
    *partidaAtual = jogoCarregado; //a partida atual se torna o jogo carregado, copiando todos os dados da struct jogoCarregado para a struct partidaAtual

    printf("\nJogo carregado com sucesso.\n");
    return 0;
}

//nessa funcao eu vou chamar o alocarVetor() e inicializar a sequencia secreta aleatoriamente dependendo do nivel
void geraSequenciaAleatoria(Jogo *partidaAtual){ 
    partidaAtual->sequenciaSecreta = alocaVetor(partidaAtual->tamanhoSequencia); //tamanho sequencia sera o tamanho do vetor (pode ser 4, 5 ou 6)

    //com isso vou preencher cada posicao da sequencia secreta com um numero aleatorio de 1 a 6
    for (int i = 0; i < partidaAtual->tamanhoSequencia; i++){
        partidaAtual->sequenciaSecreta[i] = (rand() % 6) + 1;
    }
}

void novoJogo(Jogo *partidaAtual){
    inicializaJogo(partidaAtual); //zera a struct do jogo antes de iniciar um novo jogo, para evitar lixo de memoria

    printf("\nDigite seu nome: ");
    scanf("%50s", partidaAtual->nomeJogador);
    limparBuffer(); //descarta o restante da linha se o jogador digitou mais de 50 caracteres

    definirDifuculdade(partidaAtual); //define o nivel, tamanho da sequencia e maximo de tentativas baseado na escolha do jogador

    printf("Cores disponiveis:\n1 - Vermelho\n2 - Azul\n3 - Verde\n4 - Amarelo\n5 - Roxo\n6 - Laranja\n");

    geraSequenciaAleatoria(partidaAtual); //gera a sequencia secreta aleatoria de acordo com o nivel escolhido pelo jogador

    partidaAtual->historicoJogadas = alocaMatriz(partidaAtual->maxTentativas, partidaAtual->tamanhoSequencia); //aloca a matriz de historico de jogadas

    //com tudo pronto, agora vou comecar a partida chamando o jogo() e passando o ponteiro da struct partidaAtual 
    jogo(partidaAtual);
}
void jogo(Jogo *partidaAtual){

    int *tentativaAtual = alocaVetor(partidaAtual->tamanhoSequencia); //vetor para armazenar a tentativa atual do jogador
    char *dicaAtual = malloc((partidaAtual->tamanhoSequencia + 1) * sizeof(char)); //string para armazenar a dica atual, +1 para o caractere nulo no final da string

    if (tentativaAtual == NULL || dicaAtual == NULL){
        printf("\nErro de memoria ao iniciar a rodada.\n");
        free(tentativaAtual);
        free(dicaAtual);
        return;
    }

    //exibe o historico caso seja uma partida em andamento, ou seja, se o jogador ja tiver feito alguma tentativa
    if(partidaAtual->tentativasUsadas > 0)
            exibeHistorico(partidaAtual, dicaAtual);

    while(partidaAtual->tentativasUsadas < partidaAtual->maxTentativas){ 
        zeraVetorInt(tentativaAtual, partidaAtual->tamanhoSequencia); //zera o vetor de tentativa atual para evitar lixo de memoria
        zeraVetorChar(dicaAtual, partidaAtual->tamanhoSequencia + 1 ); //zera a string de dica atual para evitar lixo de memoria

        printf("\nTentativa %d de %d", partidaAtual->tentativasUsadas + 1, partidaAtual->maxTentativas);
        printf("\nDigite %d cores (0 para voltar ao menu):", partidaAtual->tamanhoSequencia);
       
        for(int i = 0; i < partidaAtual->tamanhoSequencia; i++){
            int leitura = scanf("%d", &tentativaAtual[i]); //a variavel leitura recebe 1 se for um numero, isso vai servir para fazer validacoes

            if (leitura != 1){ //usuario digitou letra ou outro caractere que nao e numero
                limparBuffer(); //limpando para nao fazer o for entrar em loop infinito
                printf("\nValor invalido. Digite a tentativa completa novamente: ");
                zeraVetorInt(tentativaAtual, partidaAtual->tamanhoSequencia);
                printf("\nDigite %d cores (0 para voltar ao menu):", partidaAtual->tamanhoSequencia);
                i = -1; //-1 porque o for vai incrementar para 0 na proxima volta
                continue;
            }

            if(tentativaAtual[i] == 0){
                limparBuffer(); //descarta o resto da linha atual, ja que o jogador quer voltar ao menu
      
                char confirmarSaida;
                printf("\nDeseja voltar ao menu principal (S para SIM): ");
                scanf(" %c", &confirmarSaida);
                limparBuffer();
                
                if (confirmarSaida == 'S' || confirmarSaida == 's') {
                    free(tentativaAtual);
                    free(dicaAtual);
                    return; // Sai da função e volta ao menu
                } else {
                    printf("\nContinuando a tentativa...\n");
                    zeraVetorInt(tentativaAtual, partidaAtual->tamanhoSequencia);
                    printf("\nDigite %d cores (0 para voltar ao menu):", partidaAtual->tamanhoSequencia);
                    i = -1; // Reinicia a digitação da linha atual
                    continue;
                }
            }

            if (tentativaAtual[i] < 1 || tentativaAtual[i] > 6){
                limparBuffer(); //descarta o resto da linha atual, ja que a tentativa sera reiniciada
                printf("\nCor invalida na posicao %d (use 1 a 6). Digite a tentativa completa novamente.\n", i + 1);
                zeraVetorInt(tentativaAtual, partidaAtual->tamanhoSequencia);
                printf("\nDigite %d cores (0 para voltar ao menu):", partidaAtual->tamanhoSequencia);
                i = -1; //-1 porque o for vai incrementar para 0 na proxima volta
                continue;
            }
        }
        //salvando no historico de jogadas
        for(int i = 0; i < partidaAtual->tamanhoSequencia; i++)
            partidaAtual->historicoJogadas[partidaAtual->tentativasUsadas][i] = tentativaAtual[i];

        partidaAtual->tentativasUsadas++; //incrementando o contador de tentativas usadas

        exibeHistorico(partidaAtual, dicaAtual);

         //verificando se o jogador ganhou
            int win = 0;
            for (int i = 0; i < partidaAtual->tamanhoSequencia; i++){
                if (dicaAtual[i] == 'C') //conta quantas cores estao corretas
                    win++;
            }
            if(win == partidaAtual->tamanhoSequencia){ //se a quantidade de cores corretas for igual ao tamanho da sequencia, o jogador ganhou
                printf("\n\nParabéns! Você acertou a sequência secreta!\n");
                if(salvarRanking(*partidaAtual) == 0){
                    exibirRanking();
                } else {
                    printf("\nVocê não entrou no ranking. Boa sorte na próxima!\n");
                    printf("\nVoltando ao menu principal, aperte Enter para continuar...\n");
                    getchar();
                }
                liberarJogo(partidaAtual); //libera a memoria alocada para o jogo, ja que o jogador ganhou e nao vai mais jogar
                inicializaJogo(partidaAtual); //Zera o jogo para evitar que o jogador continue jogando depois de ganhar
                limparBuffer();

                free(tentativaAtual);
                free(dicaAtual);
                return;
            }

            printf("\n");
            limparBuffer(); //limpando o buffer antes de cada leitura pra evitar lixo
        }
        printf("Acabou as tentativas! A sequência secreta era: ");
        for (int i = 0; i < partidaAtual->tamanhoSequencia; i++) {
            printf("%d ", partidaAtual->sequenciaSecreta[i]);
        }
        printf("\n");

        printf("\nAperte Enter para voltar ao menu principal...\n");
        getchar();

        liberarJogo(partidaAtual); //libera a memoria alocada para o jogo, ja que o jogador ganhou e nao vai mais jogar
        inicializaJogo(partidaAtual); //Zera o jogo para evitar que o jogador continue jogando depois de perder

        free(tentativaAtual);
        free(dicaAtual);
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

//essa funcao devera comparar a tentativa do jogador com a senha e modificar a string dica. Nessa string virah primeiro o C, depois E, depois -. (ex: [C C E - -])
void comparaCombinacao(int tentativa[], int secreta[], char dica[], int tamanho){
    int usadaTentativa[tamanho]; 
    int usadaSecreta[tamanho];  //os vetores usadaTentativa e usadaSecreta servem para marcar quais posicoes ja foram usadas na comparacao, para evitar contar a mesma cor mais de uma vez
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
        if (usadaTentativa[i]) //se a posicao da tentativa ja foi usada, nao precisa comparar com a sequencia secreta, entao passa para a proxima posicao
            continue;

        for (int j = 0; j < tamanho; j++) {
            if (usadaSecreta[j]) //se a posicao da sequencia secreta ja foi usada, nao precisa comparar com a tentativa, entao passa para a proxima posicao
                continue;

            if (tentativa[i] == secreta[j]) { //se a cor da tentativa estiver na sequencia secreta, mas nao na posicao correta, adiciona um E na dica
                dica[indice++] = 'E';
                usadaTentativa[i] = 1; //marca a posicao da tentativa como usada
                usadaSecreta[j] = 1; //marca a posicao da sequencia secreta como usada
                break;
            }
        }
    }

    while (indice < tamanho) {
        dica[indice++] = '-'; //adiciona o caractere '-' na dica para as posicoes restantes que nao foram marcadas como C ou E
    }
    dica[indice] = '\0'; //adiciona \0 no final da string

    // return dica; 
}

void definirDifuculdade(Jogo *partidaAtual){
    int nivel = 0;
    int leitura;
    do{
        printf("\nEscolha o nível de dificuldade:\n");
        printf("1 - Fácil (4 cores, 10 tentativas)\n");
        printf("2 - Médio (5 cores, 12 tentativas)\n");
        printf("3 - Difícil (6 cores, 15 tentativas)\n");
        printf("Digite o nível desejado: ");
        leitura = scanf("%d", &nivel);
        limparBuffer(); //descarta o resto da linha, tenha a leitura funcionado ou nao

        if (leitura != 1){ //usuario digitou letra ou outro caractere que nao e numero
            printf("\nEntrada invalida. Digite apenas numeros.\n");
            nivel = 0;
            continue;
        }

        switch (nivel){ //switch para definir o tamanho da sequencia e o maximo de tentativas baseado na escolha de nivel do jogador
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