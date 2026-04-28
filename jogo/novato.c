#include<stdio.h>
#include <stdlib.h>
#include<string.h> //para strcpns 

//--- constantes globais
#define MAX_TERRITORIOS 50
#define TAM_STRING 100

//--- defina uma estrutura(Struct)

struct Territorio{
    char nomeDoTerritorio[TAM_STRING];
    char corDoExercito[TAM_STRING];
    int quantidadeDeTropas;
};

//--- guncao para limpar o buffer de entrada 

void limparBuffer(){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

//--- funcao principal(main)

int main(){
    struct Territorio territorios[MAX_TERRITORIOS];
    int totalTerritorios = 0;
    int opcao; 

    do {
     printf("===============================\n");
     printf(" Jogo de Estratégia\n");
     printf("===============================\n");   
     printf("1. Cadastrar novo território\n");
     printf("2. Listar territórios\n");
     printf("0. Sair\n");
     printf("===============================\n");
     printf("Escolha uma opção: ");
     scanf("%d", &opcao);
     limparBuffer();


     switch (opcao)
     {
     case 1:
        printf("Cadastrar novo território\n");
        if (totalTerritorios < MAX_TERRITORIOS) {
            printf("Digite o nome do território: ");
            fgets(territorios[totalTerritorios].nomeDoTerritorio, TAM_STRING, stdin);

            printf("Digite a cor do exército: ");
            fgets(territorios[totalTerritorios].corDoExercito, TAM_STRING, stdin);

            printf("Digite a quantidade de tropas: ");
            scanf("%d", &territorios[totalTerritorios].quantidadeDeTropas);
            

            territorios[totalTerritorios].nomeDoTerritorio[strcspn(territorios[totalTerritorios].nomeDoTerritorio, "\n")] = '\0';
            territorios[totalTerritorios].corDoExercito[strcspn(territorios[totalTerritorios].corDoExercito, "\n")] = '\0';
             
            limparBuffer();
            totalTerritorios++;

            printf("Territorio Cadastrado com sucesso!\n");

        } else {
            printf("Limite de territórios atingido!\n");
        }
        break;
        case 2:
        printf("Listar Exercitos\n");
        if(totalTerritorios == 0){
            printf("Nenhum territorio cadastrado!\n");
        }else{
            for(int i = 0; i<  totalTerritorios;i++){
                printf("Territorio %d:\n",i+1);
                printf("Nome do territorio: %s\n",territorios[i].nomeDoTerritorio);
                printf("Cor do exercito: %s\n",territorios[i].corDoExercito);
                printf("Quantidade de tropas: %d\n",territorios[i].quantidadeDeTropas);
                printf("-------------------------\n");
               
            }
        }
        printf("Pressione Enter para continuar...");
        limparBuffer();
        break;
        case 0:
        printf("Saindo do jogo...\n");
        break;
     
     default:
        printf("Opção inválida!\n");
        printf("Pressione Enter para continuar...");
        limparBuffer();
        break;
     }
    } while(opcao !=0);
    return 0;
    
};
