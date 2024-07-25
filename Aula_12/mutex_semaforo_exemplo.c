#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define maxBuffer 10000
#define maxNumero 10000
#define maxThread 300

typedef struct {
  int buffer[maxBuffer]; // local de armazenamento circular
  int in,     // posicao de insercao
      out,    // posicao de retirada
      count,  // quantidade de elementos no buffer
      tam;    // tamanho util do buffer
} tBuffer;

tBuffer buffer;

int nProd, // quantidade de dados a serem fabricados (producoes)
    nCons, // quantidade de dados a serem consumidos (consumos),
           // que nesta implementacao eh igual ao numero de producoes
    ntp,   // numero de threads produtoras
    ntc;   // numero de threads consumidoras

pthread_mutex_t inMutex = PTHREAD_MUTEX_INITIALIZER,
                outMutex = PTHREAD_MUTEX_INITIALIZER;

sem_t emptySlots, fullSlots, prodCount, consCount;

// inicializa a estrutura do buffer
void criaBuffer(int tam)
{
    buffer.in = -1;
    buffer.out = -1;
    buffer.count = 0;
    buffer.tam = tam;
}

// deposita o dado no buffer
int deposita(int dado)
{
    sem_wait(&emptySlots);

    pthread_mutex_lock(&inMutex);

    buffer.in = (buffer.in + 1) % buffer.tam;
    buffer.buffer[buffer.in] = dado;
    buffer.count++;

    pthread_mutex_unlock(&inMutex);

    sem_post(&fullSlots);

    return 1;
}

// retira o dado do buffer
int retira(int *dado)
{
    sem_wait(&fullSlots);

    pthread_mutex_lock(&outMutex);

    buffer.out = (buffer.out + 1) % buffer.tam;
    *dado = buffer.buffer[buffer.out];
    buffer.count--;

    pthread_mutex_unlock(&outMutex);

    sem_post(&emptySlots);

    return 1;
}

// lista os elementos do buffer
void mostraBuffer(void)
{
    int i, in, out;

    in = buffer.in;
    out = buffer.out;

    printf("\nSituacao do Buffer:");
    printf("\nin=%d", in);
    printf("\nout=%d", out);

    printf("\nElementos Restantes no Buffer: ");

    while (in != out)
    {
        printf("%d, ", buffer.buffer[out]);
        out = (out + 1) % buffer.tam;
    }
}

// simula trabalho da thread com tempo "aleatorio"
void trabalha(void)
{
    int i, j, n, m;
    double soma;

    /*
    trecho de codigo que simula o consumo de tempo
    de forma parcialmente aleatoria. Teste retirar
    o codigo do comentario(ativar o codigo).

    srand(time(NULL));
    n = 1000+(rand()%1000);
    m = 5000+(rand()%5000);
    soma = rand();
    for (i=0; i<n; i++)
      {   soma=((soma+(i*2)/2))*(rand()/soma);
          for (j=0; j<m; j++)
              soma=soma-rand()+soma;
      }
    */
}

// codigo da thread produtora
void *produtor(void *ptr)
{
    int *id, dado, sinal;

    id = (int *)ptr;

    srand(time(NULL));

    while (1)
    {
        sem_wait(&prodCount);

        trabalha();

        printf("\nProdutor %d: vai produzir um dado", *id);

        // produz um dado positivo ou negativo aleatoriamente
        dado = rand() % maxNumero;
        sinal = (rand() % 2) * 2 - 1;
        dado = sinal * dado;

        printf("\nProdutor %d: produziu o dado %d", *id, dado);

        if (deposita(dado))
        {
            printf("\nProdutor %d: depositou o dado %d", *id, dado);

            sem_post(&consCount);
        }
        else
            printf("\nProdutor %d: Nao depositou!", *id);

        printf("\nProdutor %d: numero de producoes restantes = %d", *id, nProd - buffer.count);

        if (buffer.count == 0 && sem_trywait(&prodCount) != 0)
        {
            printf("\nProdutor %d: Producoes Finalizadas!!!", *id);
            break;
        }
    }

    return NULL;
}

// codigo da thread consumidora
void *consumidor(void *ptr)
{
    int *id, dado;

    id = (int *)ptr;

    while (1)
    {
        sem_wait(&consCount);

        trabalha();

        printf("\nConsumidor %d: vai consumir um dado", *id);

        if (retira(&dado))
        {
            printf("\nConsumidor %d: consumiu o dado %d", *id, dado);

            sem_post(&prodCount);
        }
        else
            printf("\nConsumidor %d: Nao consumiu!", *id);

        printf("\nConsumidor %d: numero de consumos restantes = %d", *id, nCons - buffer.count);

        if (buffer.count == nProd && sem_trywait(&consCount) != 0)
        {
            printf("\nConsumidor %d: Consumos Finalizados!!!", *id);
            break;
        }
    }

    return NULL;
}

// codigo da thread primaria, principal
int main()
{
    pthread_t tProd[maxThread], tCons[maxThread];
    int iretProd[maxThread], iretCons[maxThread],
        tp[maxThread], tc[maxThread], resp,
        i, pindex, cindex, tam;

    do
    {
        printf("\nProblema do Produtor-Consumidor Multithreaded\n");

        do
        {
            printf("\nQual o tamanho util do buffer? (max = %d) => ", maxBuffer);
            scanf("%d", &(tam));
        } while ((tam < 1) || (tam > maxBuffer));

        criaBuffer(tam);

        do
        {
            printf("\nQuantas threads produtoras? (max=%d) => ", maxThread - 1);
            scanf("%d", &ntp);
        } while ((ntp < 1) || (ntp >= maxThread));

        for (i = 0; i < ntp; i++)
            tp[i] = i;

        do
        {
            printf("\nQuantas threads consumidoras? (max=%d) => ", maxThread - ntp);
            scanf("%d", &ntc);
        } while ((ntc < 1) || (ntp + ntc > maxThread));

        for (i = 0; i < ntc; i++)
            tc[i] = i;

        do
        {
            printf("\nQuantas producoes deseja? => ");
            scanf("%d", &nProd);
        } while (nProd < 1);

        nCons = nProd;

        sem_init(&emptySlots, 0, tam);
        sem_init(&fullSlots, 0, 0);
        sem_init(&prodCount, 0, nProd);
        sem_init(&consCount, 0, 0);

        /* Cria threads produtoras e consumidoras de forma intercalada
           para balancear as atividades de producao e consumo */

        pindex = 0;
        cindex = 0;
        do
        {
            if (pindex < ntp)
            {
                iretProd[pindex] = pthread_create(&tProd[pindex], NULL, produtor, (void *)&tp[pindex]);
                pindex++;
            }

            if (cindex < ntc)
            {
                iretCons[cindex] = pthread_create(&tCons[cindex], NULL, consumidor, (void *)&tc[cindex]);
                cindex++;
            }
        } while ((pindex < ntp) || (cindex < ntc));

        trabalha();

        // espera todas as threads acabarem

        for (pindex = 0; pindex < ntp; pindex++)
            pthread_join(tProd[pindex], NULL);
        for (cindex = 0; cindex < ntc; cindex++)
            pthread_join(tCons[cindex], NULL);

        mostraBuffer();

        sem_destroy(&emptySlots);
        sem_destroy(&fullSlots);
        sem_destroy(&prodCount);
        sem_destroy(&consCount);

        printf("\nDeseja Nova Execucao, <1>Sim <2>Nao? => ");
        scanf("%d", &resp);

    } while (resp == 1);

    return 0;
}
