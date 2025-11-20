/* jogo_territorios.c
   Arquivo único que implementa:
   - struct Territorio
   - cadastro (5 territórios) com alocação dinâmica (calloc)
   - função atacar(Territorio* atacante, Territorio* defensor)
   - sistema de missões sorteadas, armazenadas com malloc
   - verificação de missão após cada turno
   - loop de jogo com alternância de jogadores
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N_TERRITORIOS 5
#define MAX_NOME 30
#define MAX_COR 10
#define MAX_MISSAO 150

typedef struct Territorio {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

/* Protótipos */
void exibirMapa(Territorio* mapa, int n);
void atacar(Territorio* atacante, Territorio* defensor);
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao_com_cor, Territorio* mapa, int tamanho); 
/* nota: missao_com_cor armazena "texto_da_missao|cor_do_jogador" para identificar o jogador */

/* MAIN */
int main() {
    srand((unsigned) time(NULL));

    int n = N_TERRITORIOS;

    /* aloca dinamicamente o mapa */
    Territorio* mapa = (Territorio*) calloc(n, sizeof(Territorio));
    if (!mapa) {
        fprintf(stderr, "Erro ao alocar memória para o mapa.\n");
        return 1;
    }

    printf("=== Cadastro de %d Territórios ===\n\n", n);
    for (int i = 0; i < n; i++) {
        printf("Território %d:\n", i + 1);

        printf("Nome: ");
        scanf(" %[^\n]", mapa[i].nome);

        printf("Cor do exército (ex: vermelho, azul): ");
        scanf(" %[^\n]", mapa[i].cor);

        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);

        if (mapa[i].tropas < 1) mapa[i].tropas = 1; /* garantir no mínimo 1 */
        printf("\n");
    }

    /* definir jogadores (2 jogadores) */
    char jogador1Nome[30], jogador2Nome[30];
    char jogador1Cor[MAX_COR], jogador2Cor[MAX_COR];

    printf("=== Defina os jogadores ===\n");
    printf("Nome do Jogador 1: ");
    scanf(" %[^\n]", jogador1Nome);
    printf("Cor do Jogador 1: ");
    scanf(" %[^\n]", jogador1Cor);

    printf("Nome do Jogador 2: ");
    scanf(" %[^\n]", jogador2Nome);
    printf("Cor do Jogador 2: ");
    scanf(" %[^\n]", jogador2Cor);

    /* vetor de missões (pelo menos 5) - textos sem referência a cores */
    char* missoes[] = {
        "Conquistar 3 territorios",
        "Ter ao menos 10 tropas em um territorio",
        "Controlar 2 territorios sequenciais",
        "Eliminar todas as tropas de algum adversario",
        "Ter ao menos 6 tropas no total entre todos seus territorios"
    };
    int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);

    /* aloca e atribui missões dinamicamente para cada jogador */
    char* missaoJ1 = (char*) malloc(MAX_MISSAO);
    char* missaoJ2 = (char*) malloc(MAX_MISSAO);
    if (!missaoJ1 || !missaoJ2) {
        fprintf(stderr, "Erro ao alocar memória para missões.\n");
        free(mapa);
        return 1;
    }

    atribuirMissao(missaoJ1, missoes, totalMissoes);
    /* anexa uma tag com a cor do jogador para que verificarMissao saiba identificar o dono */
    strcat(missaoJ1, "|");
    strcat(missaoJ1, jogador1Cor);

    atribuirMissao(missaoJ2, missoes, totalMissoes);
    strcat(missaoJ2, "|");
    strcat(missaoJ2, jogador2Cor);

    printf("\n=== Missões Atribuídas ===\n");
    /* mostra apenas o texto da missão (antes do '|') */
    char *sep;
    sep = strchr(missaoJ1, '|');
    if (sep) { *sep = '\0'; } 
    printf("%s: %s\n", jogador1Nome, missaoJ1);
    if (sep) { *sep = '|'; } /* restaura */

    sep = strchr(missaoJ2, '|');
    if (sep) { *sep = '\0'; }
    printf("%s: %s\n", jogador2Nome, missaoJ2);
    if (sep) { *sep = '|'; }

    /* exibe mapa inicial */
    printf("\n=== Mapa Inicial ===\n");
    exibirMapa(mapa, n);

    /* loop de jogo: alterna entre jogador 1 e 2 */
    int jogadorAtual = 1; /* 1 ou 2 */
    int atacanteIdx, defensorIdx;
    int sair = 0;

    while (!sair) {
        char* nomeAtual = (jogadorAtual == 1) ? jogador1Nome : jogador2Nome;
        char* corAtual  = (jogadorAtual == 1) ? jogador1Cor  : jogador2Cor;

        printf("\n--- Turno de %s (cor: %s) ---\n", nomeAtual, corAtual);

        /* mostra mapa */
        exibirMapa(mapa, n);

        /* peça para escolher um território atacante pertencente ao jogador atual */
        printf("Escolha índice do território ATACANTE (ou -1 para sair): ");
        scanf("%d", &atacanteIdx);
        if (atacanteIdx == -1) { sair = 1; break; }
        if (atacanteIdx < 0 || atacanteIdx >= n) {
            printf("Índice inválido. Tente novamente.\n");
            continue;
        }
        /* valida propriedade */
        if (strcmp(mapa[atacanteIdx].cor, corAtual) != 0) {
            printf("Esse território não pertence a %s. Escolha um território seu.\n", nomeAtual);
            continue;
        }
        /* valida tropas para atacar (precisa ter pelo menos 2 para atacar) */
        if (mapa[atacanteIdx].tropas < 2) {
            printf("Território precisa ter pelo menos 2 tropas para atacar. Tropas atuais: %d\n", mapa[atacanteIdx].tropas);
            continue;
        }

        /* escolher defensor (pode ser de qualquer cor exceto a mesma) */
        printf("Escolha índice do território DEFENSOR: ");
        scanf("%d", &defensorIdx);
        if (defensorIdx < 0 || defensorIdx >= n) {
            printf("Índice inválido. Tente novamente.\n");
            continue;
        }
        if (defensorIdx == atacanteIdx) {
            printf("Um território não pode atacar ele mesmo.\n");
            continue;
        }

        /* realizar ataque */
        atacar(&mapa[atacanteIdx], &mapa[defensorIdx]);

        /* exibir mapa após ataque */
        printf("\n=== Mapa após ataque ===\n");
        exibirMapa(mapa, n);

        /* verificar missões (ambos jogadores) */
        if (verificarMissao(missaoJ1, mapa, n)) {
            printf("\n>>> %s (cor %s) CUMPRIU A MISSÃO: ", jogador1Nome, jogador1Cor);
            /* imprime texto da missão (antes do '|') */
            char saved = 0;
            char* p = strchr(missaoJ1, '|');
            if (p) { saved = *p; *p = '\0'; }
            printf("%s\n", missaoJ1);
            if (p) *p = saved;
            break;
        }
        if (verificarMissao(missaoJ2, mapa, n)) {
            printf("\n>>> %s (cor %s) CUMPRIU A MISSÃO: ", jogador2Nome, jogador2Cor);
            char saved = 0;
            char* p = strchr(missaoJ2, '|');
            if (p) { saved = *p; *p = '\0'; }
            printf("%s\n", missaoJ2);
            if (p) *p = saved;
            break;
        }

        /* alterna jogador */
        jogadorAtual = (jogadorAtual == 1) ? 2 : 1;
    }

    /* libera memória */
    free(mapa);
    free(missaoJ1);
    free(missaoJ2);

    printf("\nFim do jogo. Obrigado por jogar!\n");
    return 0;
}

/* exibirMapa: imprime índice, nome, cor e tropas de cada território */
void exibirMapa(Territorio* mapa, int n) {
    printf("\n=== Territórios ===\n");
    for (int i = 0; i < n; i++) {
        printf("%d - %s | Cor: %s | Tropas: %d\n", i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

/* atacar: simula um ataque com rolagem de um dado para cada lado (1-6)
   Regras simples:
   - atacante e defensor rolam 1 dado
   - maior vence
   - perdedor perde 1 tropa
   - se defensor ficar com 0 tropas -> é conquistado: cor passa para atacante->cor e defensor.tropas = 1
   - atacante não pode reduzir abaixo de 1 tropa (mantemos pelo menos 1)
*/
void atacar(Territorio* atacante, Territorio* defensor) {
    int dado_atk = rand() % 6 + 1;
    int dado_def = rand() % 6 + 1;

    printf("\n--- Batalha: %s (atk - %s, %d tropa(s)) vs %s (def - %s, %d tropa(s)) ---\n",
           atacante->nome, atacante->cor, atacante->tropas,
           defensor->nome, defensor->cor, defensor->tropas);

    printf("Atacante rolou: %d\n", dado_atk);
    printf("Defensor rolou: %d\n", dado_def);

    if (dado_atk > dado_def) {
        printf("Atacante venceu a rolagem!\n");
        defensor->tropas -= 1;
        if (defensor->tropas <= 0) {
            printf("Território %s conquistado por %s!\n", defensor->nome, atacante->cor);
            defensor->tropas = 1;
            /* copia cor do atacante para o defensor */
            strncpy(defensor->cor, atacante->cor, MAX_COR-1);
            defensor->cor[MAX_COR-1] = '\0';
        }
    } else {
        printf("Defensor venceu a rolagem!\n");
        atacante->tropas -= 1;
        if (atacante->tropas < 1) atacante->tropas = 1; /* garantir pelo menos 1 */
    }
}

/* atribuirMissao: sorteia uma missão e copia para destino (sem anexar cor)
   assinatura conforme requisitada: void atribuirMissao(char* destino, char* missoes[], int totalMissoes)
*/
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int idx = rand() % totalMissoes;
    strncpy(destino, missoes[idx], MAX_MISSAO - 1);
    destino[MAX_MISSAO - 1] = '\0';
}

/* verificarMissao: verifica se a missão foi cumprida.
   Recebe 'missao_com_cor' no formato: "texto_da_missao|cor_do_jogador"
   Retorna 1 se cumprida, 0 caso contrário.
   Implementa verificações simples para os 5 tipos definidos no vetor de missões.
*/
int verificarMissao(char* missao_com_cor, Territorio* mapa, int tamanho) {
    char copia[MAX_MISSAO];
    strncpy(copia, missao_com_cor, MAX_MISSAO-1);
    copia[MAX_MISSAO-1] = '\0';

    /* separar texto da missão da cor do jogador */
    char* pipe = strchr(copia, '|');
    if (!pipe) return 0; /* formato inesperado */
    *pipe = '\0';
    char* textoMissao = copia;
    char* corJogador = pipe + 1;

    /* normalizar: remover possíveis espaços no início/fim (simples) */
    while (*textoMissao == ' ') textoMissao++;

    /* Verificações por texto */
    if (strstr(textoMissao, "Conquistar 3 territorios") != NULL) {
        int cont = 0;
        for (int i = 0; i < tamanho; i++)
            if (strcmp(mapa[i].cor, corJogador) == 0) cont++;
        return (cont >= 3);
    }

    if (strstr(textoMissao, "Ter ao menos 10 tropas") != NULL ||
        strstr(textoMissao, "Ter ao menos 10 tropas em um territorio") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0 && mapa[i].tropas >= 10)
                return 1;
        }
        return 0;
    }

    if (strstr(textoMissao, "Controlar 2 territorios sequenciais") != NULL ||
        strstr(textoMissao, "Controlar 2 territorios seguidos") != NULL) {
        for (int i = 0; i < tamanho - 1; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0 && strcmp(mapa[i+1].cor, corJogador) == 0)
                return 1;
        }
        return 0;
    }

    if (strstr(textoMissao, "Eliminar todas as tropas de algum adversario") != NULL) {
        /* verifica se existe ao menos uma cor diferente que tenha 0 tropas no total */
        /* primeiro coletar cores diferentes da do jogador */
        for (int i = 0; i < tamanho; i++) {
            char corAtual[MAX_COR];
            strncpy(corAtual, mapa[i].cor, MAX_COR-1);
            corAtual[MAX_COR-1] = '\0';
            if (strcmp(corAtual, corJogador) == 0) continue;

            /* soma tropas dessa cor */
            int soma = 0;
            for (int j = 0; j < tamanho; j++) {
                if (strcmp(mapa[j].cor, corAtual) == 0) soma += mapa[j].tropas;
            }
            if (soma == 0) return 1;
        }
        return 0;
    }

    if (strstr(textoMissao, "Ter ao menos 6 tropas no total") != NULL ||
        strstr(textoMissao, "Ter ao menos 6 tropas no total entre todos seus territorios") != NULL) {
        int soma = 0;
        for (int i = 0; i < tamanho; i++)
            if (strcmp(mapa[i].cor, corJogador) == 0) soma += mapa[i].tropas;
        return (soma >= 6);
    }

    /* se missão desconhecida, retorna 0 por padrão */
    return 0;
}
