#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

typedef struct Territorio Territorio;

// Função de ataque
void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\n=== INICIANDO ATAQUE ===\n");
    printf("%s (%s) ATACANDO %s (%s)\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    int dado_atk = rand() % 6 + 1;
    int dado_def = rand() % 6 + 1;

    printf("Dado do atacante: %d\n", dado_atk);
    printf("Dado do defensor: %d\n", dado_def);

    if (dado_atk > dado_def) {
        printf("Atacante venceu a rolagem!\n");
        defensor->tropas--;

        if (defensor->tropas <= 0) {
            printf("\n>>> TERRITÓRIO CONQUISTADO!!! <<<\n");
            defensor->tropas = 1;
            strcpy(defensor->cor, atacante->cor);
        }
    } else {
        printf("Defensor venceu a rolagem!\n");
        atacante->tropas--;
        if (atacante->tropas < 1)
            atacante->tropas = 1;
    }
}

int main() {
    srand(time(NULL));

    int n = 5; // cadastro fixo de 5 territórios
    Territorio* t = (Territorio*) malloc(n * sizeof(Territorio));

    printf("=== CADASTRO DE TERRITÓRIOS ===\n\n");

    for (int i = 0; i < n; i++) {
        printf("Território %d:\n", i + 1);

        printf("Nome: ");
        scanf(" %[^\n]", t[i].nome);

        printf("Cor do exército: ");
        scanf(" %[^\n]", t[i].cor);

        printf("Quantidade de tropas: ");
        scanf("%d", &t[i].tropas);

        printf("\n");
    }

    printf("\n=== TERRITÓRIOS CADASTRADOS ===\n");
    for (int i = 0; i < n; i++) {
        printf("%d - %s | Cor: %s | Tropas: %d\n",
               i, t[i].nome, t[i].cor, t[i].tropas);
    }

    int atacante, defensor;

    // Loop para ataques
    while (1) {
        printf("\nEscolha o território ATACANTE (-1 para sair): ");
        scanf("%d", &atacante);

        if (atacante == -1)
            break;

        printf("Escolha o território DEFENSOR: ");
        scanf("%d", &defensor);

        if (atacante == defensor) {
            printf("Um território não pode atacar ele mesmo.\n");
            continue;
        }

        atacar(&t[atacante], &t[defensor]);

        // Exibe após ataque
        printf("\n=== SITUAÇÃO ATUAL DOS TERRITÓRIOS ===\n");
        for (int i = 0; i < n; i++) {
            printf("%d - %s | Cor: %s | Tropas: %d\n",
                   i, t[i].nome, t[i].cor, t[i].tropas);
        }
    }

    free(t);
    return 0;
}
