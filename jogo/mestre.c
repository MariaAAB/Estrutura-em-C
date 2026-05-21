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

typedef struct {
    char nome[TAM_STRING];
    char cor[TAM_STRING];
    char* missao; // alocada dinamicamente
} Player;

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

// --- Missões
// vetor estático de descrições de missões
char* missoesDisponiveis[] = {
    "Conquistar 2 territorios",
    "Conquistar 3 territorios",
    "Eliminar todas as tropas da cor Vermelho",
    "Possuir o territorio Fortaleza",
    "Ter mais tropas que qualquer outra cor"
};
int totalMissoesDisponiveis = sizeof(missoesDisponiveis)/sizeof(missoesDisponiveis[0]);

// atribui uma missão escolhida aleatoriamente para destino (que deve ter memória alocada)
void atribuirMissao(char* destino, char* missoes[], int totalMissoes){
    if (!destino || totalMissoes <= 0) return;
    int idx = rand() % totalMissoes;
    strcpy(destino, missoes[idx]);
}

// verificar missão simples: retorna 1 se cumprida, 0 caso contrário
int verificarMissao(char* missao, Territorio* mapa, int tamanho, char* corJogador){
    if (!missao || !mapa || tamanho <= 0 || !corJogador) return 0;

    if (strcmp(missao, "Conquistar 2 territorios") == 0){
        int cont = 0;
        for (int i=0;i<tamanho;i++) if (strcmp(mapa[i].corDoExercito, corJogador)==0) cont++;
        return cont >= 2;
    }
    if (strcmp(missao, "Conquistar 3 territorios") == 0){
        int cont = 0;
        for (int i=0;i<tamanho;i++) if (strcmp(mapa[i].corDoExercito, corJogador)==0) cont++;
        return cont >= 3;
    }
    if (strcmp(missao, "Eliminar todas as tropas da cor Vermelho") == 0){
        for (int i=0;i<tamanho;i++){
            if (strcmp(mapa[i].corDoExercito, "Vermelho") == 0 && mapa[i].quantidadeDeTropas > 0) return 0;
        }
        return 1;
    }
    if (strcmp(missao, "Possuir o territorio Fortaleza") == 0){
        for (int i=0;i<tamanho;i++){
            if (strcmp(mapa[i].nomeDoTerritorio, "Fortaleza") == 0 && strcmp(mapa[i].corDoExercito, corJogador)==0) return 1;
        }
        return 0;
    }
    if (strcmp(missao, "Ter mais tropas que qualquer outra cor") == 0){
        // soma tropas por cor simples: calcula tropas do jogador e verifica se é >= tropas de quaisquer outros
        int tropasJogador = 0;
        // para simplicidade, percorrer e somar por cor
        for (int i=0;i<tamanho;i++) if (strcmp(mapa[i].corDoExercito, corJogador)==0) tropasJogador += mapa[i].quantidadeDeTropas;
        // agora verificar se existe cor com mais tropas
        // simplificar: para cada território de outra cor, acumular por cor em arrays temporários
        char outrasCores[20][TAM_STRING];
        int tropasPorCor[20];
        int coresEncontradas = 0;
        for (int i=0;i<tamanho;i++){
            char* c = mapa[i].corDoExercito;
            if (strcmp(c, corJogador) == 0) continue;
            int idx = -1;
            for (int j=0;j<coresEncontradas;j++) if (strcmp(outrasCores[j], c)==0) idx = j;
            if (idx == -1){
                idx = coresEncontradas;
                strcpy(outrasCores[coresEncontradas], c);
                tropasPorCor[coresEncontradas] = 0;
                coresEncontradas++;
            }
            tropasPorCor[idx] += mapa[i].quantidadeDeTropas;
        }
        for (int j=0;j<coresEncontradas;j++){
            if (tropasPorCor[j] > tropasJogador) return 0;
        }
        return 1;
    }

    return 0; // missão desconhecida
}

int main(){
    Territorio* territorios = NULL;
    int capacidade = 0;
    int totalTerritorios = 0;
    int opcao;
    Player* players = NULL;
    int capacidadePlayers = 0;
    int totalPlayers = 0;

    srand((unsigned) time(NULL));

    do {
        printf("===============================\n");
        printf(" Jogo de Estratégia\n");
        printf("===============================\n");
        printf("1. Inicializar vetor de territórios\n");
        printf("2. Cadastrar novo território\n");
        printf("3. Listar territórios\n");
        printf("4. Atacar\n");
        printf("5. Criar jogador\n");
        printf("6. Atribuir missão a jogador\n");
        printf("7. Verificar missões\n");
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
                // verificar missões após o ataque
                for (int p=0;p<totalPlayers;p++){
                    if (players[p].missao){
                        if (verificarMissao(players[p].missao, territorios, totalTerritorios, players[p].cor)){
                            printf("Jogador %s cumpriu a missão: %s\n", players[p].nome, players[p].missao);
                            printf("Vencedor: %s (cor: %s)\n", players[p].nome, players[p].cor);
                            // encerrar programa após vitória
                            if (territorios) free(territorios);
                            for (int i=0;i<totalPlayers;i++) if (players[i].missao) free(players[i].missao);
                            if (players) free(players);
                            return 0;
                        }
                    }
                }
                break;
            }
            case 5:{
                printf("Criar jogador\n");
                printf("Quantos jogadores deseja alocar? ");
                int n;
                if (scanf("%d", &n) != 1){ limparBuffer(); break; }
                limparBuffer();
                if (n <= 0){ printf("Número inválido.\n"); break; }
                if (players){
                    for (int i=0;i<totalPlayers;i++) if (players[i].missao) free(players[i].missao);
                    free(players);
                }
                players = (Player*) calloc(n, sizeof(Player));
                if (!players){ printf("Falha na alocação de jogadores.\n"); capacidadePlayers = 0; totalPlayers = 0; }
                else { capacidadePlayers = n; totalPlayers = 0; printf("Espaço para %d jogadores criado.\n", n); }
                break;
            }
            case 6:{
                if (!players || capacidadePlayers == 0){ printf("Crie jogadores primeiro (opção 5).\n"); break; }
                if (totalPlayers >= capacidadePlayers){ printf("Capacidade de jogadores atingida.\n"); break; }
                printf("Nome do jogador: ");
                fgets(players[totalPlayers].nome, TAM_STRING, stdin);
                players[totalPlayers].nome[strcspn(players[totalPlayers].nome, "\n")] = '\0';
                printf("Cor do jogador: ");
                fgets(players[totalPlayers].cor, TAM_STRING, stdin);
                players[totalPlayers].cor[strcspn(players[totalPlayers].cor, "\n")] = '\0';
                // aloca missão dinamicamente e atribui
                players[totalPlayers].missao = (char*) malloc(TAM_STRING);
                if (players[totalPlayers].missao) atribuirMissao(players[totalPlayers].missao, missoesDisponiveis, totalMissoesDisponiveis);
                totalPlayers++;
                printf("Jogador criado e missão atribuída.\n");
                break;
            }
            case 7:{
                if (!players || totalPlayers==0){ printf("Nenhum jogador cadastrado.\n"); break; }
                if (!territorios || totalTerritorios==0){ printf("Mapa vazio.\n"); break; }
                for (int p=0;p<totalPlayers;p++){
                    printf("Jogador: %s, Cor: %s, Missao: %s\n", players[p].nome, players[p].cor, players[p].missao?players[p].missao:"(nenhuma)");
                    if (players[p].missao){
                        int ok = verificarMissao(players[p].missao, territorios, totalTerritorios, players[p].cor);
                        printf("Missão cumprida? %s\n", ok?"SIM":"NAO");
                    }
                }
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
    if (players){
        for (int i=0;i<totalPlayers;i++) if (players[i].missao) free(players[i].missao);
        free(players);
    }
    return 0;
}
