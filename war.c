#include <stdio.h>

struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

int main() {
    struct Territorio t[5];

    printf("=== Cadastro de Territórios ===\n\n");

    for (int i = 0; i < 5; i++) {
        printf("Território %d:\n", i + 1);

        printf("Nome: ");
        scanf(" %[^\n]", t[i].nome);

        printf("Cor do exército: ");
        scanf(" %[^\n]", t[i].cor);

        printf("Quantidade de tropas: ");
        scanf("%d", &t[i].tropas);

        printf("\n");
    }

    printf("\n=== Territórios Registrados ===\n\n");

    for (int i = 0; i < 5; i++) {
        printf("Território %d:\n", i + 1);
        printf("Nome: %s\n", t[i].nome);
        printf("Cor do exército: %s\n", t[i].cor);
        printf("Tropas: %d\n", t[i].tropas);
        printf("-------------------------\n");
    }

    return 0;
}