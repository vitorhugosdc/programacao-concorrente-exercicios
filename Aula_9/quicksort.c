#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define maxVet 10000
#define maxNumero 10000

typedef int tipoVetor[maxVet];

tipoVetor A;
int tamA;
int minIntervalo;

typedef struct {
    int *V;
    int ini;
    int fim;
} QuickSortArgs;

int leTam(void) {
    int tam;
    do {
        printf("\nDigite o tamanho do Vetor = ");
        scanf("%d", &tam);
    } while ((tam < 1) || (tam > maxVet));
    return tam;
}

int leMinIntervalo(void) {
    int intervalo;
    do {
        printf("\nDigite o intervalo minimo para criar uma nova thread = ");
        scanf("%d", &intervalo);
    } while (intervalo < 1);
    return intervalo;
}

void geraVetor(int *V, int tam) {
    int i, sinal;
    srand(time(NULL));
    printf("\nGeracao Aleatoria do Vetor\n");
    for (i = 0; i < tam; i++) {
        V[i] = rand() % maxNumero;
        sinal = (rand() % 2) * 2 - 1;
        V[i] = sinal * V[i];
    }
}

void mostraVetor(char *id, int *V, int tam) {
    int i;
    printf("\nElementos do Vetor %s:\n", id);
    printf("%d", V[0]);
    for (i = 1; i < tam; i++) {
        printf(", %d", V[i]);
    }
    printf("\n");
}

int particiona(int *V, int ini, int fim) {
    int posDivisor, divisor, aux, i, meio;
    meio = (ini + fim) / 2;
    aux = V[fim];
    V[fim] = V[meio];
    V[meio] = aux;
    divisor = V[fim];
    posDivisor = ini;
    for (i = ini; i < fim; i++) {
        if (V[i] < divisor) {
            aux = V[i];
            V[i] = V[posDivisor];
            V[posDivisor] = aux;
            posDivisor++;
        }
    }
    aux = V[posDivisor];
    V[posDivisor] = V[fim];
    V[fim] = aux;
    return posDivisor;
}

void* quickSort(void* args) {
    QuickSortArgs* qsArgs = (QuickSortArgs*) args;
    int *V = qsArgs->V;
    int ini = qsArgs->ini;
    int fim = qsArgs->fim;
    int posDivisor;

    if (fim > ini) {
        posDivisor = particiona(V, ini, fim);
        
        if (posDivisor - 1 - ini > minIntervalo) {
            pthread_t leftThread;
            QuickSortArgs* leftArgs = malloc(sizeof(QuickSortArgs));
            leftArgs->V = V;
            leftArgs->ini = ini;
            leftArgs->fim = posDivisor - 1;
            pthread_create(&leftThread, NULL, quickSort, leftArgs);
            pthread_join(leftThread, NULL);
            free(leftArgs);
        } else {
            QuickSortArgs leftArgs = { .V = V, .ini = ini, .fim = posDivisor - 1 };
            quickSort(&leftArgs);
        }

        if (fim - posDivisor - 1 > minIntervalo) {
            pthread_t rightThread;
            QuickSortArgs* rightArgs = malloc(sizeof(QuickSortArgs));
            rightArgs->V = V;
            rightArgs->ini = posDivisor + 1;
            rightArgs->fim = fim;
            pthread_create(&rightThread, NULL, quickSort, rightArgs);
            pthread_join(rightThread, NULL);
            free(rightArgs);
        } else {
            QuickSortArgs rightArgs = { .V = V, .ini = posDivisor + 1, .fim = fim };
            quickSort(&rightArgs);
        }
    }

    return NULL;
}

int main(void) {
    int resp;
    do {
        tamA = leTam();
        minIntervalo = leMinIntervalo();
        geraVetor(A, tamA);
        mostraVetor("A Original", A, tamA);
        printf("\nOrdenando A por Quick Sort ...\n");

        QuickSortArgs args = { .V = A, .ini = 0, .fim = tamA - 1 };
        quickSort(&args);

        mostraVetor("A Ordenado", A, tamA);
        printf("\nDeseja Nova Execucao, <1>Sim <2>Nao? => ");
        scanf("%d", &resp);
    } while (resp == 1);

    return 0;
}
