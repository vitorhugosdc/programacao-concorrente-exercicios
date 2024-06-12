/*Ordene um vetor com n elementos inteiros usando 2 threads. Faça a leitura do n e gere o vetor aleatoriamente. 
Um thread procura pelos menores e vai colocando no início do vetor e a outra thread procura pelos maiores e vai colocando no final do vetor. 
Ambas as threads trabalham até a metade do vetor, não sobrepondo posições de ordenação.

OBS: na seleção direta, a cada passo, procura-se o menor (ou maior) elemento, 
comparando todos os elementos ainda não ordenados e o coloca na sua posição de ordenação. 
Repete o processo até ordenar todo o vetor. */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

int N;
int *vect;

struct tipoPack {
    int id;
    int start;
    int end;
    int *vect;
};

typedef struct tipoPack tipoPack;

void *funcaoOrdenaVetor(void *ptr)
{
    tipoPack *pack = (tipoPack *) ptr;
    int start = pack->start;
    int end = pack->end;
    int *vect = pack->vect;

    for(int i = start; i < end; i++) {
        for(int j = i + 1; j < end; j++) {
            if(vect[j] < vect[i]) {
                int temp = vect[i];
                vect[i] = vect[j];
                vect[j] = temp;
            }
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    if(argc != 2 || (atoi(argv[1]) < 1 || atoi(argv[1]) > 100)) {
        printf("Use: %s <tamanho_vetor>\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    vect = (int *)malloc(N * sizeof(int));

    pthread_t thread[2];
    tipoPack pack[2];

    srand(time(NULL));

    for(int i = 0; i < N; i++) {
        vect[i] = rand() % 100;
    }

    printf("Vetor original: ");
    for(int i = 0; i < N; i++) {
        printf("%d ", vect[i]);
    }
    printf("\n\n");

    pack[0].id = 0;
    pack[0].start = 0;
    pack[0].end = N / 2;
    pack[0].vect = vect;

    pack[1].id = 1;
    pack[1].start = N / 2;
    pack[1].end = N;
    pack[1].vect = vect;

    for (int i = 0; i < 2; i++) {
        pthread_create(&(thread[i]), NULL, funcaoOrdenaVetor, (void*)&pack[i]);
    }

    for (int i = 0; i < 2; i++) {
        pthread_join(thread[i], NULL);
    }

    int temp[N];
    int i = 0, j = N / 2, k = 0;
    while(i < N / 2 && j < N) {
        if(vect[i] < vect[j]) {
            temp[k++] = vect[i++];
        } else {
            temp[k++] = vect[j++];
        }
    }
    while(i < N / 2) {
        temp[k++] = vect[i++];
    }
    while(j < N) {
        temp[k++] = vect[j++];
    }
    for(i = 0; i < N; i++) {
        vect[i] = temp[i];
    }

    printf("Vetor ordenado: ");
    for(int i = 0; i < N; i++) {
        printf("%d ", vect[i]);
    }
    printf("\n");

    free(vect);

    return 0;
}
