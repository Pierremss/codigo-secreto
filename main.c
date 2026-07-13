#include "jogo.h"

int main(void){
    srand(time(NULL)); //semente para gerar numeros aleatorios
    char opcao = ' ';

    //Lembrete para nao esquecer de iniciar a struct
    do{
        Jogo partidaAtual;
        inicializaJogo(&partidaAtual);

        exibeMenuPrincipal();

        printf("\nDigite a opção desejada: ");
        scanf(" %c", &opcao);
        limparBuffer();

        switch (opcao){
            case 'A':
            case 'a':
                exibeAjuda();
                getchar();
                break;
            case 'N':
            case 'n':
                novoJogo(&partidaAtual);
                break;
            case 'C':
            case 'c':
                //carregaJogo();
                break;
            case 'S':
            case 's':
                //salvaJogo();
                break;
            case 'R':
            case 'r':
                exibeRanking();
                break;
            case 'X':
            case 'x':
            //preciso verificar se essa logica realmente esta certa, e se o jogador tentar jogar outor jogo antes de sair? 
                printf("\nSaindo do jogo...\n");
                liberaMatriz(partidaAtual.historicoJogadas, partidaAtual.maxTentativas);
                free(partidaAtual.sequenciaSecreta);
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
        }
    } while (opcao != 'X' && opcao != 'x');
}
