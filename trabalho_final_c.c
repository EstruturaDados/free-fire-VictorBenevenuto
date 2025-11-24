#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMPONENTES 20

// Estrutura que representa um componente
typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade;
} Componente;

// Estrutura para armazenar resultados de ordenação
typedef struct {
    long comparacoes;
    double tempo;
} ResultadoOrdenacao;

// Função para limpar o buffer de entrada
void limparEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Função para ler um texto seguro
void lerTexto(char *dest, int tam) {
    fgets(dest, tam, stdin);
    size_t n = strlen(dest);
    if (n > 0 && dest[n - 1] == '\n') dest[n - 1] = '\0';
}

// Pausa da tela
void pausar() {
    printf("\nPressione ENTER para continuar...");
    limparEntrada();
}

// Trocar posições
void swap(Componente *a, Componente *b) {
    Componente temp = *a;
    *a = *b;
    *b = temp;
}

// Mostrar lista completa
void mostrarLista(Componente v[], int n) {
    printf("\nLISTA DE COMPONENTES (%d itens)\n", n);
    printf("-----------------------------------------------------------\n");
    printf("%-30s %-15s %s\n", "Nome", "Tipo", "Prioridade");
    printf("-----------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("%-30s %-15s %d\n", v[i].nome, v[i].tipo, v[i].prioridade);
    }
}

// Ordenação por nome (Bubble Sort)
void ordenarPorNome(Componente v[], int n, long *comp) {
    long c = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            c++;
            if (strcmp(v[j].nome, v[j+1].nome) > 0) {
                swap(&v[j], &v[j + 1]);
            }
        }
    }
    *comp = c;
}

// Ordenação por tipo (Insertion Sort)
void ordenarPorTipo(Componente v[], int n, long *comp) {
    long c = 0;
    for (int i = 1; i < n; i++) {
        Componente chave = v[i];
        int j = i - 1;

        while (j >= 0 && strcmp(v[j].tipo, chave.tipo) > 0) {
            c++;
            v[j + 1] = v[j];
            j--;
        }

        v[j + 1] = chave;
    }
    *comp = c;
}

// Ordenação por prioridade (Selection Sort)
void ordenarPorPrioridade(Componente v[], int n, long *comp) {
    long c = 0;
    for (int i = 0; i < n - 1; i++) {
        int menor = i;

        for (int j = i + 1; j < n; j++) {
            c++;
            if (v[j].prioridade < v[menor].prioridade) menor = j;
        }

        swap(&v[i], &v[menor]);
    }
    *comp = c;
}

// Busca binária pelo nome
int buscaBinaria(Componente v[], int n, const char *chave, long *comp) {
    int inicio = 0, fim = n - 1;
    long c = 0;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        c++;
        int r = strcmp(v[meio].nome, chave);

        if (r == 0) {
            *comp = c;
            return meio;
        }

        if (r < 0) inicio = meio + 1;
        else fim = meio - 1;
    }

    *comp = c;
    return -1;
}

// Medir tempo e comparações
ResultadoOrdenacao medirOrdenacao(void (*f)(Componente[], int, long*), Componente v[], int n) {
    ResultadoOrdenacao r;
    long comp;

    clock_t inicio = clock();
    f(v, n, &comp);
    clock_t fim = clock();

    r.comparacoes = comp;
    r.tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    return r;
}

// Cadastro de componentes
int cadastrar(Componente v[], int *n) {
    if (*n >= MAX_COMPONENTES) {
        printf("Limite máximo atingido!\n");
        return 0;
    }

    Componente c;

    printf("Nome: ");
    lerTexto(c.nome, sizeof(c.nome));

    printf("Tipo: ");
    lerTexto(c.tipo, sizeof(c.tipo));

    char buffer[10];
    printf("Prioridade (1-10): ");
    lerTexto(buffer, sizeof(buffer));
    c.prioridade = atoi(buffer);

    v[*n] = c;
    (*n)++;
    return 1;
}

// Exemplo automático
void exemplo(Componente v[], int *n) {
    Componente base[] = {
        {"Motor", "Propulsao", 2},
        {"Sensor", "Controle", 5},
        {"Chip", "Logica", 1},
        {"Painel", "Estrutura", 3}
    };

    int qtd = sizeof(base) / sizeof(base[0]);

    for (int i = 0; i < qtd && *n < MAX_COMPONENTES; i++) {
        v[*n] = base[i];
        (*n)++;
    }
}

// ---------------------------------------------------------
// PROGRAMA PRINCIPAL
// ---------------------------------------------------------
int main() {
    Componente lista[MAX_COMPONENTES];
    int n = 0;
    int ordenadoPorNome = 0;

    char opcao[5];

    while (1) {
        printf("\n========= MENU =========\n");
        printf("1 - Cadastrar componente\n");
        printf("2 - Mostrar lista\n");
        printf("3 - Ordenar lista\n");
        printf("4 - Buscar por nome\n");
        printf("5 - Inserir exemplo\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        lerTexto(opcao, sizeof(opcao));

        int escolha = atoi(opcao);

        if (escolha == 1) {
            cadastrar(lista, &n);
            ordenadoPorNome = 0;
        }
        else if (escolha == 2) {
            mostrarLista(lista, n);
        }
        else if (escolha == 3) {
            printf("Ordenar por:\n1 - Nome\n2 - Tipo\n3 - Prioridade\nEscolha: ");
            lerTexto(opcao, sizeof(opcao));
            int ord = atoi(opcao);

            ResultadoOrdenacao r;

            if (ord == 1) {
                r = medirOrdenacao(ordenarPorNome, lista, n);
                ordenadoPorNome = 1;
            }
            else if (ord == 2) {
                r = medirOrdenacao(ordenarPorTipo, lista, n);
                ordenadoPorNome = 0;
            }
            else if (ord == 3) {
                r = medirOrdenacao(ordenarPorPrioridade, lista, n);
                ordenadoPorNome = 0;
            }

            printf("Comparações: %ld | Tempo: %.6f s\n", r.comparacoes, r.tempo);
        }
        else if (escolha == 4) {
            if (!ordenadoPorNome) {
                printf("A lista precisa estar ordenada por NOME antes da busca!\n");
                continue;
            }

            char chave[30];
            printf("Nome para buscar: ");
            lerTexto(chave, sizeof(chave));

            long comp;
            int pos = buscaBinaria(lista, n, chave, &comp);

            printf("Comparações realizadas: %ld\n", comp);

            if (pos >= 0) {
                printf("Encontrado: %s | %s | %d\n", lista[pos].nome, lista[pos].tipo, lista[pos].prioridade);
            } else {
                printf("Componente não encontrado.\n");
            }
        }
        else if (escolha == 5) {
            exemplo(lista, &n);
        }
        else if (escolha == 0) {
            break;
        }

        pausar();
    }

    return 0;
}