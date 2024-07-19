#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define maxBuffer 10000
#define maxNumero 10000
#define maxThread 300

typedef struct {
  int buffer[maxBuffer];
  int in, out, count, tam;
  pthread_mutex_t lock;
} tBuffer;

tBuffer buffer;

int nProd, nCons, ntp, ntc;
pthread_mutex_t prod_lock;
pthread_mutex_t cons_lock;

void criaBuffer(int tam) {
    buffer.in = -1;
    buffer.out = -1;
    buffer.count = 0;
    buffer.tam = tam;
    pthread_mutex_init(&buffer.lock, NULL);
}

int deposita(int dado) {
    pthread_mutex_lock(&buffer.lock);
    int success = 0;
    if (buffer.count != buffer.tam) {
        buffer.in = (buffer.in + 1) % buffer.tam;
        buffer.buffer[buffer.in] = dado;
        buffer.count++;
        success = 1;
    }
    pthread_mutex_unlock(&buffer.lock);
    return success;
}

int retira(int *dado) {
    pthread_mutex_lock(&buffer.lock);
    int success = 0;
    if (buffer.count != 0) {
        buffer.out = (buffer.out + 1) % buffer.tam;
        *dado = buffer.buffer[buffer.out];
        buffer.count--;
        success = 1;
    }
    pthread_mutex_unlock(&buffer.lock);
    return success;
}

void mostraBuffer(void) {
    int in, out;

    pthread_mutex_lock(&buffer.lock);
    in = buffer.in;
    out = buffer.out;

    printf("\nSituacao do Buffer:");
    printf("\nin=%d", in);
    printf("\nout=%d", out);

    printf("\nElementos Restantes no Buffer: ");
    while (in != out) {
        printf("%d, ", buffer.buffer[out]);
        out = (out + 1) % buffer.tam;
    }
    pthread_mutex_unlock(&buffer.lock);
}

void trabalha(void) {
    srand(time(NULL));
    int n = 1000 + (rand() % 1000);
    int m = 5000 + (rand() % 5000);
    double soma = rand();
    for (int i = 0; i < n; i++) {
        soma = ((soma + (i * 2) / 2)) * (rand() / soma);
        for (int j = 0; j < m; j++) {
            soma = soma - rand() + soma;
        }
    }
}

void *produtor(void *ptr) {
    int *id = (int *)ptr;
    int dado, sinal;

    srand(time(NULL));

    while (1) {
        trabalha();

        printf("\nProdutor %d: vai produzir um dado", *id);

        dado = rand() % maxNumero;
        sinal = (rand() % 2) * 2 - 1;
        dado = sinal * dado;

        printf("\nProdutor %d: produziu o dado %d", *id, dado);

        pthread_mutex_lock(&prod_lock);
        if (nProd <= 0) {
            pthread_mutex_unlock(&prod_lock);
            break;
        }
        if (deposita(dado)) {
            printf("\nProdutor %d: depositou o dado %d", *id, dado);
            nProd--;
            if (nProd == 0) {
                printf("\nProdutor %d: Producoes Finalizadas!!!", *id);
            }
        } else {
            printf("\nProdutor %d: Nao depositou!", *id);
        }
        pthread_mutex_unlock(&prod_lock);

        printf("\nProdutor %d: numero de producoes = %d", *id, nProd);
    }
    return NULL;
}

void *consumidor(void *ptr) {
    int *id = (int *)ptr;
    int dado;

    while (1) {
        trabalha();

        printf("\nConsumidor %d: vai consumir um dado", *id);

        pthread_mutex_lock(&cons_lock);
        if (nCons <= 0) {
            pthread_mutex_unlock(&cons_lock);
            break;
        }
        if (retira(&dado)) {
            printf("\nConsumidor %d: consumiu o dado %d", *id, dado);
            nCons--;
            if (nCons == 0) {
                printf("\nConsumidor %d: Consumos Finalizados!!!", *id);
            }
        } else {
            printf("\nConsumidor %d: Nao consumiu!", *id);
        }
        pthread_mutex_unlock(&cons_lock);

        printf("\nConsumidor %d: numero de consumos = %d", *id, nCons);
    }
    return NULL;
}

int main() {
    pthread_t tProd[maxThread], tCons[maxThread];
    int iretProd[maxThread], iretCons[maxThread], tp[maxThread], tc[maxThread], resp, i, pindex, cindex, tam;

    pthread_mutex_init(&prod_lock, NULL);
    pthread_mutex_init(&cons_lock, NULL);

    do {
        printf("\nProblema do Produtor-Consumidor Multithreaded\n");

        do {
            printf("\nQual o tamanho util do buffer? (max = %d) => ", maxBuffer);
            scanf("%d", &tam);
        } while ((tam < 1) || (tam > maxBuffer));

        criaBuffer(tam);

        do {
            printf("\nQuantas threads produtoras? (max=%d) => ", maxThread - 1);
            scanf("%d", &ntp);
        } while ((ntp < 1) || (ntp >= maxThread));

        for (i = 0; i < ntp; i++) {
            tp[i] = i;
        }

        do {
            printf("\nQuantas threads consumidoras? (max=%d) => ", maxThread - ntp);
            scanf("%d", &ntc);
        } while ((ntc < 1) || (ntp + ntc > maxThread));

        for (i = 0; i < ntc; i++) {
            tc[i] = i;
        }

        do {
            printf("\nQuantas producoes deseja? => ");
            scanf("%d", &nProd);
        } while (nProd < 1);

        nCons = nProd;

        pindex = 0;
        cindex = 0;
        do {
            if (pindex < ntp) {
                iretProd[pindex] = pthread_create(&tProd[pindex], NULL, produtor, (void *)&tp[pindex]);
                pindex++;
            }

            if (cindex < ntc) {
                iretCons[cindex] = pthread_create(&tCons[cindex], NULL, consumidor, (void *)&tc[cindex]);
                cindex++;
            }
        } while ((pindex < ntp) || (cindex < ntc));

        trabalha();

        for (pindex = 0; pindex < ntp; pindex++) {
            pthread_join(tProd[pindex], NULL);
        }
        for (cindex = 0; cindex < ntc; cindex++) {
            pthread_join(tCons[cindex], NULL);
        }

        mostraBuffer();

        printf("\nDeseja Nova Execucao, <1>Sim <2>Nao? => ");
        scanf("%d", &resp);

    } while (resp == 1);

    pthread_mutex_destroy(&prod_lock);
    pthread_mutex_destroy(&cons_lock);
    pthread_mutex_destroy(&buffer.lock);

    return 0;
}
