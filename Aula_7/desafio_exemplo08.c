#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    int *vect;
    int inicio;
    int fim;
    int x;
    int *result;
} ThreadData;

int result = -1;

void *thread_func(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    for (int i = data->inicio; i < data->fim && *data->result == -1; i++) {
        if (data->vect[i] == data->x) {
            *data->result = i;
            return NULL;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s <tamanho_vetor> <num_threads> <valor>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int k = atoi(argv[2]);
    int x = atoi(argv[3]);

    if (k > n) {
        printf("Numero de threads deve ser maior que o tamanho do vetor.\n");
        return 1;
    }

    int vect[n];
    pthread_t threads[k];
    ThreadData thread_data[k];
    
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        vect[i] = rand() % 100;
    }

    printf("Vetor: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", vect[i]);
    }
    printf("\n");

    int tamanho_segmento = n / k;

    for (int i = 0; i < k; i++) {
        thread_data[i].vect = vect;
        thread_data[i].inicio = i * tamanho_segmento;
        thread_data[i].fim = (i == k - 1) ? n : (i + 1) * tamanho_segmento;
        thread_data[i].x = x;
        thread_data[i].result = &result;
        pthread_create(&threads[i], NULL, thread_func, &thread_data[i]);
    }

    for (int i = 0; i < k; i++) {
        pthread_join(threads[i], NULL);
    }

    if (result != -1) {
        printf("Valor %d encontrado no indice %d\n", x, result);
    } else {
        printf("Valor %d nao encontrado no vetor\n", x);
    }

    return 0;
}
