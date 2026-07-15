#include "jogo.h"
#include "ranking.h"

int main(void){
    srand(time(NULL)); //semente para gerar numeros aleatorios
    char opcao = ' ';
    Jogo partidaAtual;
    inicializaJogo(&partidaAtual);

    do{

        exibeMenuPrincipal();

        printf("\nDigite a opção desejada: ");
        scanf(" %c", &opcao);
        limparBuffer();

        switch (opcao){
            case 'V':
            case 'v':
                if(partidaAtual.nivel == 0){
                    printf("\nNenhum jogo em andamento. Por favor, inicie um novo jogo.\n");
                    getchar();
                }else{
                    printf("Voltando ao jogo em andamento...\n");
                    jogo(&partidaAtual);
                }
                break;
            case 'A':
            case 'a':
                exibeAjuda();
                getchar();
                break;
            case 'N':
            case 'n':
                liberarJogo(&partidaAtual); //liberando a memoria do jogo anterior antes de iniciar um novo
                novoJogo(&partidaAtual);
                break;
            case 'C':
            case 'c':
                //carregaJogo();
                break;
            case 'S':
            case 's':
                if(partidaAtual.nivel == 0){
                    printf("\nNenhum jogo em andamento para salvar.\n");
                }else{
                    salvaJogo(&partidaAtual);
                    limparBuffer();
                    getchar();
                }
                break;
            case 'R':
            case 'r':
                exibirRanking();
                getchar();
                break;
            case 'X':
            case 'x':
            //preciso verificar se essa logica realmente esta certa, e se o jogador tentar jogar outor jogo antes de sair? 
                printf("\nSaindo do jogo...\n");
                liberarJogo(&partidaAtual);
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
        }
    } while (opcao != 'X' && opcao != 'x');
}
