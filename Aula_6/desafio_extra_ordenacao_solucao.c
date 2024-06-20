#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

/*
THREADS:

    pthread_t thread;

    iret = pthread_create(&thread, NULL, funcao_da_thread, (void*) &pack);

    pthread_join(thread, NULL);


TEMPO:

    time_t tempoSeg;

    struct tm *tempoLocal;

    time(&tempoSeg);

    tempoLocal=localtime(&tempoSeg);

    asctime(tempoLocal);


ALEATORIO:

    srand(time(NULL));

    rand()


*/

#define maxElem 100

typedef int tipoVetor[maxElem];

tipoVetor vetorA, vetorAux;
int tam;

void *ordenaSelecao(void *ptr);

void geraVetor(tipoVetor V, int tam)
{   int i;

    for (i=0; i<tam; i++)
        V[i]=rand()%100;
}

void mostraVetor(char *ident, tipoVetor V, int tam)
{   int i;

    printf("\nVetor %s: ", ident);
    for (i=0; i<tam; i++)
       printf("%d ", V[i]);
    printf("\n");
}

void copiaVetor(tipoVetor V1, int ini, int fim, tipoVetor V2)
{   int i;

    for (i=ini; i<=fim; i++)
        V2[i]=V1[i];
}

void main(void)
{   int id1=1, id2=2, iret1, iret2;

    pthread_t thread1, thread2;


    srand(time(NULL));

    printf("\nDigite o tamanho do vetor => ");
    scanf("%d", &tam);

    geraVetor(vetorA, tam);
    mostraVetor("Original", vetorA, tam);

    copiaVetor(vetorA, 0, tam-1, vetorAux);
    mostraVetor("Copia", vetorAux, tam);

    iret1 = pthread_create(&thread1, NULL, ordenaSelecao, (void*) &id1);
    iret2 = pthread_create(&thread2, NULL, ordenaSelecao, (void*) &id2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    mostraVetor("A Ordenado Parcialmente", vetorA, tam);
    mostraVetor("Aux Ordenado Parcialmente", vetorAux, tam);

    copiaVetor(vetorAux, tam/2, tam-1, vetorA);

    mostraVetor("Ordenado", vetorA, tam);

}

void *ordenaSelecao(void *ptr)
{   int *id, posIns, posAfrente, posPraTras, posMenor, posMaior, aux;

    id=(int *)ptr;

    if (*id==1)
    {
        for (posIns=0; posIns<tam/2; posIns++)
        {
            posMenor=posIns;
            for (posAfrente=posIns+1; posAfrente<tam; posAfrente++)
            {
                if (vetorA[posMenor]>vetorA[posAfrente])
                   posMenor=posAfrente;

            }
            aux=vetorA[posIns];
            vetorA[posIns]=vetorA[posMenor];
            vetorA[posMenor]=aux;
        }
    }
    else
    {
        for (posIns=tam-1; posIns>=tam/2; posIns--)
        {
            posMaior=posIns;
            for (posPraTras=posIns-1; posPraTras>0; posPraTras--)
            {
                if (vetorAux[posMaior]<vetorAux[posPraTras])
                   posMaior=posPraTras;

            }
            aux=vetorAux[posIns];
            vetorAux[posIns]=vetorAux[posMaior];
            vetorAux[posMaior]=aux;
        }
    }
}