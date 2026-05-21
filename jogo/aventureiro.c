#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//--- constantes globais
#define MAX_TERRITORIOS 50
#define TAM_STRING 100

typedef struct {
    char nomeDoTerritorio[TAM_STRING];
    char corDoExercito[TAM_STRING];
    int quantidadeDeTropas;
} Territorio;

// limpa buffer de entrada
void limparBuffer(){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

// função que simula um ataque entre territórios
void atacar(Territorio* atacante, Territorio* defensor){
    if (!atacante || !defensor) return;
    if (atacante->quantidadeDeTropas <= 0){
        printf("Atacante não tem tropas suficientes para atacar.\n");
        return;
    }
    if (defensor->quantidadeDeTropas <= 0){
        printf("Defensor não tem tropas — conquista automática!\n");
        // transfere controle sem combates
        strcpy(defensor->corDoExercito, atacante->corDoExercito);
        defensor->quantidadeDeTropas = atacante->quantidadeDeTropas / 2;
        atacante->quantidadeDeTropas -= defensor->quantidadeDeTropas;
        return;
    }

    int ataque = rand() % (atacante->quantidadeDeTropas) + 1; // 1..tropas
    int defesa  = rand() % (defensor->quantidadeDeTropas) + 1;  // 1..tropas

    printf("Rolagem: atacante=%d vs defensor=%d\n", ataque, defesa);

    if (ataque > defesa){
        printf("Atacante vence! %s toma %s\n", atacante->corDoExercito, defensor->nomeDoTerritorio);
        // atualiza dono e tropas: metade das tropas atacantes ocupam o território
        int tropasTomadas = atacante->quantidadeDeTropas / 2;
        if (tropasTomadas < 1) tropasTomadas = 1;
        strcpy(defensor->corDoExercito, atacante->corDoExercito);
        defensor->quantidadeDeTropas = tropasTomadas;
        atacante->quantidadeDeTropas -= tropasTomadas;
        if (atacante->quantidadeDeTropas < 0) atacante->quantidadeDeTropas = 0;
    } else {
        printf("Defensor resiste! %s mantém %s\n", defensor->corDoExercito, defensor->nomeDoTerritorio);
        // perda proporcional de tropas no atacante
        int perdas = defesa / 2;
        if (perdas < 1) perdas = 1;
        atacante->quantidadeDeTropas -= perdas;
        if (atacante->quantidadeDeTropas < 0) atacante->quantidadeDeTropas = 0;
    }

    // exibição resumida após o combate
    printf("Estado pós-ataque:\n");
    printf("%s - Cor: %s, Tropas: %d\n", atacante->nomeDoTerritorio, atacante->corDoExercito, atacante->quantidadeDeTropas);
    printf("%s - Cor: %s, Tropas: %d\n", defensor->nomeDoTerritorio, defensor->corDoExercito, defensor->quantidadeDeTropas);
}

int main(){
    Territorio* territorios = NULL;
    int capacidade = 0;
    int totalTerritorios = 0;
    int opcao;

    srand((unsigned) time(NULL));

    do {
        printf("===============================\n");
        printf(" Jogo de Estratégia\n");
        printf("===============================\n");
        printf("1. Inicializar vetor de territórios\n");
        printf("2. Cadastrar novo território\n");
        printf("3. Listar territórios\n");
        printf("4. Atacar\n");
        printf("0. Sair\n");
        printf("===============================\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1) { limparBuffer(); opcao = -1; }
        limparBuffer();

        switch (opcao){
            case 1:{
                if (territorios) free(territorios);
                printf("Quantos territórios deseja alocar (max %d)? ", MAX_TERRITORIOS);
                int n;
                if (scanf("%d", &n) != 1) { limparBuffer(); n = 0; }
                limparBuffer();
                if (n <= 0 || n > MAX_TERRITORIOS){
                    printf("Número inválido.\n");
                    territorios = NULL;
                    capacidade = 0;     
                    totalTerritorios = 0;
                } else {
                    territorios = (Territorio*) calloc(n, sizeof(Territorio));
                    if (!territorios){
                        printf("Falha na alocação de memória.\n");
                        capacidade = 0;
                    } else {
                        capacidade = n;
                        totalTerritorios = 0;
                        printf("Vetor de %d territórios alocado com sucesso.\n", n);
                    }
                }
                break;
            }
            case 2:{
                if (!territorios){
                    printf("Vetor não inicializado. Use a opção 1 primeiro.\n");
                    break;
                }
                if (totalTerritorios >= capacidade){
                    printf("Capacidade atingida.\n");
                    break;
                }
                printf("Cadastrar novo território\n");
                printf("Digite o nome do território: ");
                fgets(territorios[totalTerritorios].nomeDoTerritorio, TAM_STRING, stdin);
                printf("Digite a cor do exército: ");
                fgets(territorios[totalTerritorios].corDoExercito, TAM_STRING, stdin);
                printf("Digite a quantidade de tropas: ");
                if (scanf("%d", &territorios[totalTerritorios].quantidadeDeTropas) != 1){
                    territorios[totalTerritorios].quantidadeDeTropas = 0;
                }
                territorios[totalTerritorios].nomeDoTerritorio[strcspn(territorios[totalTerritorios].nomeDoTerritorio, "\n")] = '\0';
                territorios[totalTerritorios].corDoExercito[strcspn(territorios[totalTerritorios].corDoExercito, "\n")] = '\0';
                limparBuffer();
                totalTerritorios++;
                printf("Território cadastrado.\n");
                break;
            }
            case 3:{
                printf("Listar Exercitos\n");
                if (!territorios || totalTerritorios == 0){
                    printf("Nenhum território cadastrado!\n");
                } else {
                    for(int i = 0; i < totalTerritorios; i++){
                        printf("Território %d:\n", i+1);
                        printf("Nome: %s\n", territorios[i].nomeDoTerritorio);
                        printf("Cor do exército: %s\n", territorios[i].corDoExercito);
                        printf("Quantidade de tropas: %d\n", territorios[i].quantidadeDeTropas);
                        printf("-------------------------\n");
                    }
                }
                printf("Pressione Enter para continuar...");
                limparBuffer();
                break;
            }
            case 4:{
                if (!territorios || totalTerritorios < 2){
                    printf("É necessário pelo menos 2 territórios cadastrados para atacar.\n");
                    break;
                }
                int a, d;
                printf("Escolha o índice do território atacante (1..%d): ", totalTerritorios);
                if (scanf("%d", &a) != 1){ limparBuffer(); break; }
                printf("Escolha o índice do território defensor (1..%d): ", totalTerritorios);
                if (scanf("%d", &d) != 1){ limparBuffer(); break; }
                limparBuffer();
                if (a < 1 || a > totalTerritorios || d < 1 || d > totalTerritorios || a == d){
                    printf("Índices inválidos.\n");
                    break;
                }
                atacar(&territorios[a-1], &territorios[d-1]);
                break;
            }
            case 0:{
                printf("Saindo do jogo...\n");
                break;
            }
            default:{
                printf("Opção inválida!\n");
                printf("Pressione Enter para continuar...");
                limparBuffer();
                break;
            }
        }

    } while(opcao != 0);

    if (territorios) free(territorios);
    return 0;
}
