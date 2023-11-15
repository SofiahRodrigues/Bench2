#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void gerarAN(char *n_arquivo, int quantidade) {
    FILE *arquivo = fopen(n_arquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    srand(time(NULL));
    for (int i = 0; i < quantidade; i++) {
        int numero = rand() % 1000 + 1;
        fprintf(arquivo, "%d\n", numero);
    }

    fclose(arquivo);
}

double buscaLR(int numero, char *n_arquivo, FILE *arquivo) {
    int num;
    if (fscanf(arquivo, "%d", &num) == EOF) {
        return -1.0;
    }

    if (num == numero) {
        return 0.0;
    }

    double tempo = buscaLR(numero, n_arquivo, arquivo);
    if (tempo == -1.0) {
        return -1.0;
    }

    return tempo + 1.0;
}

double buscaL(int numero, char *n_arquivo) {
    FILE *arquivo = fopen(n_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    double start_time = (double)clock() / CLOCKS_PER_SEC;

    double tempo = buscaLR(numero, n_arquivo, arquivo);

    double end_time = (double)clock() / CLOCKS_PER_SEC;

    fclose(arquivo);

    if (tempo != -1.0) {
        return tempo / CLOCKS_PER_SEC;
    } else {
        return -1.0;
    }
}

double buscaSR(int numero, char *n_arquivo, FILE *arquivo) {
    int num;
    if (fscanf(arquivo, "%d", &num) == EOF || num == -1) {
        return -1.0;
    }

    if (num == numero) {
        return 0.0;
    }

    double tempo = buscaSR(numero, n_arquivo, arquivo);
    if (tempo == -1.0) {
        return -1.0;
    }

    return tempo + 1.0;
}

double buscaS(int numero, char *n_arquivo) {
    FILE *arquivo = fopen(n_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    fseek(arquivo, 0, SEEK_END);
    fprintf(arquivo, "%d\n", -1);
    fseek(arquivo, 0, SEEK_SET);

    double start_time = (double)clock() / CLOCKS_PER_SEC;

    double tempo = buscaSR(numero, n_arquivo, arquivo);

    double end_time = (double)clock() / CLOCKS_PER_SEC;

    fclose(arquivo);

    if (tempo != -1.0) {
        return tempo / CLOCKS_PER_SEC;
    } else {
        return -1.0;
    }
}

double buscaBR(int numero, int *numeros, int esquerda, int direita) {
    if (esquerda > direita) {
        return -1.0;
    }

    int meio = esquerda + (direita - esquerda) / 2;

    if (numeros[meio] == numero) {
        return 0.0;
    } else if (numeros[meio] < numero) {
        return buscaBR(numero, numeros, meio + 1, direita);
    } else {
        return buscaBR(numero, numeros, esquerda, meio - 1);
    }
}

double buscaB(int numero, char *n_arquivo) {
    FILE *arquivo = fopen(n_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    int *numeros = NULL;
    int tamanho = 0;

    int num;
    while (fscanf(arquivo, "%d", &num) != EOF) {
        tamanho++;
        numeros = (int *)realloc(numeros, tamanho * sizeof(int));
        numeros[tamanho - 1] = num;
    }

    fclose(arquivo);

    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = i + 1; j < tamanho; j++) {
            if (numeros[i] > numeros[j]) {
                int temp = numeros[i];
                numeros[i] = numeros[j];
                numeros[j] = temp;
            }
        }
    }

    double start_time = (double)clock() / CLOCKS_PER_SEC;

    double tempo = buscaBR(numero, numeros, 0, tamanho - 1);

    double end_time = (double)clock() / CLOCKS_PER_SEC;

    free(numeros);

    if (tempo != -1.0) {
        return tempo / CLOCKS_PER_SEC;
    } else {
        return -1.0;
    }
}

void insertion_sort(int *numeros, int tamanho) {
    for (int i = 1; i < tamanho; i++) {
        int chave = numeros[i];
        int j = i - 1;

        while (j >= 0 && numeros[j] > chave) {
            numeros[j + 1] = numeros[j];
            j--;
        }

        numeros[j + 1] = chave;
    }
}

void bubble_sort(int *numeros, int tamanho) {
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = 0; j < tamanho - i - 1; j++) {
            if (numeros[j] > numeros[j + 1]) {
                int temp = numeros[j];
                numeros[j] = numeros[j + 1];
                numeros[j + 1] = temp;
            }
        }
    }
}

int part(int *numeros, int baixo, int alto) {
    int pivo = numeros[alto];
    int i = (baixo - 1);

    for (int j = baixo; j <= alto - 1; j++) {
        if (numeros[j] < pivo) {
            i++;
            int temp = numeros[i];
            numeros[i] = numeros[j];
            numeros[j] = temp;
        }
    }

    int temp = numeros[i + 1];
    numeros[i + 1] = numeros[alto];
    numeros[alto] = temp;

    return (i + 1);
}

void quick_sort(int *numeros, int baixo, int alto) {
    if (baixo < alto) {
        int pivo = part(numeros, baixo, alto);

        quick_sort(numeros, baixo, pivo - 1);
        quick_sort(numeros, pivo + 1, alto);
    }
}

int main() {
    char nome_arquivo[] = "numeros.txt";
    int quantidade;

    printf("Quantidade de números a gerar: ");
    scanf("%d", &quantidade);
    gerarAN(nome_arquivo, quantidade);

    while (1) {
        printf("\nMenu de Operações:\n");
        printf("1. Busca de elemento\n");
        printf("2. Sair\n");
        printf("Escolha a operação (1/2): ");
        int escolha;
        scanf("%d", &escolha);

        if (escolha == 1) {
            int numero;
            printf("Digite o número a ser encontrado: ");
            scanf("%d", &numero);

            double busca_linear_tempo = buscaL(numero, nome_arquivo);
            double busca_sentinela_tempo = buscaS(numero, nome_arquivo);
            double busca_binaria_tempo = buscaB(numero, nome_arquivo);

            if (busca_linear_tempo != -1.0) {
                printf("Tempo da busca linear: %f segundos\n", busca_linear_tempo);
            } else {
                printf("Número não encontrado na busca linear.\n");
            }

            if (busca_sentinela_tempo != -1.0) {
                printf("Tempo da busca sentinela: %f segundos\n", busca_sentinela_tempo);
            } else {
                printf("Número não encontrado na busca sentinela.\n");
            }

            if (busca_binaria_tempo != -1.0) {
                printf("Tempo da busca binária: %f segundos\n", busca_binaria_tempo);
            } else {
                printf("Número não encontrado na busca binária.\n");
            }
        } else if (escolha == 2) {
            FILE *arquivo = fopen(nome_arquivo, "r");
            if (arquivo == NULL) {
                printf("Erro ao abrir o arquivo.\n");
                exit(1);
            }

            int numeros[quantidade];
            int i = 0;
            int num;
            while (fscanf(arquivo, "%d", &num) != EOF) {
                numeros[i] = num;
                i++;
            }
            fclose(arquivo);

            int numeros_insertion[quantidade];
            int numeros_bubble[quantidade];
            int numeros_quick[quantidade];
            for (int j = 0; j < quantidade; j++) {
                numeros_insertion[j] = numeros[j];
                numeros_bubble[j] = numeros[j];
                numeros_quick[j] = numeros[j];
            }

            double start_time_insertion = (double)clock() / CLOCKS_PER_SEC;
            insertion_sort(numeros_insertion, quantidade);
            double end_time_insertion = (double)clock() / CLOCKS_PER_SEC;

            double start_time_bubble = (double)clock() / CLOCKS_PER_SEC;
            bubble_sort(numeros_bubble, quantidade);
            double end_time_bubble = (double)clock() / CLOCKS_PER_SEC;

            double start_time_quick = (double)clock() / CLOCKS_PER_SEC;
            quick_sort(numeros_quick, 0, quantidade - 1);
            double end_time_quick = (double)clock() / CLOCKS_PER_SEC;

            printf("Tempo do Insertion Sort: %f segundos\n", end_time_insertion - start_time_insertion);
            printf("Tempo do Bubble Sort: %f segundos\n", end_time_bubble - start_time_bubble);
            printf("Tempo do Quick Sort: %f segundos\n", end_time_quick - start_time_quick);
        } else if (escolha == 3) {
            break;
        } else {
            printf("Por favor, escolha 1, 2 ou 3.\n");
        }
    }

    return 0;
}
