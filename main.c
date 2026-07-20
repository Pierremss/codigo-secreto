// Nome: Pierre Miguel Silveira Silva Franco
// Matrícula: 26.1.4061

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
                    printf("\nAperte Enter para voltar ao menu principal...\n");
                    getchar();
                }else{
                    printf("Voltando ao jogo em andamento...\n");
                    jogo(&partidaAtual);
                }
                break;
            case 'A':
            case 'a':
                exibeAjuda();
                printf("\nAperte Enter para voltar ao menu principal...\n");
                getchar();
                break;
            case 'N':
            case 'n':
                liberarJogo(&partidaAtual); //liberando a memoria do jogo anterior antes de iniciar um novo
                novoJogo(&partidaAtual);
                break;
            case 'C':
            case 'c':
                if (carregaJogo(&partidaAtual) == 0) {
                    jogo(&partidaAtual);
                }
                break;
            case 'S':
            case 's':
                if(partidaAtual.nivel == 0){
                    printf("\nNenhum jogo em andamento para salvar.\n");
                    printf("\nAperte Enter para voltar ao menu principal...\n");
                    getchar();
                }else{
                    salvaJogo(&partidaAtual);
                    limparBuffer();
                    getchar();
                }
                break;
            case 'R':
            case 'r':
                exibirRanking();
                break;
            case 'X':
            case 'x':
                if (partidaAtual.nivel != 0) { //ver se tem jogo em andamento
                    char desejaSalvar;
                    printf("\nVocê tem um jogo em andamento. Deseja salvar antes de sair? (S para SIM): ");
                    scanf(" %c", &desejaSalvar);
                    limparBuffer();
                    if (desejaSalvar == 'S' || desejaSalvar == 's') {
                        salvaJogo(&partidaAtual);
                    }
                }
                printf("\nSaindo do jogo...\n");
                liberarJogo(&partidaAtual);
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
                printf("\nAperte Enter para voltar ao menu principal...\n");
                getchar();
        }
    } while (opcao != 'X' && opcao != 'x');
}