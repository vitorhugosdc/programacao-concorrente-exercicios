/*

Exemplo 11: Implementa o metodo de ordenacao MergeSort multithread e recursiva,
com limite minimo no tamanho do segmento. O MergeSort divide o vetor em 2 segmentos
e para cada um executa novamente o MergeSort, repetindo o processo de divisao
enquanto for possivel. Nessa versao paralela, os segmentos esquerdo e direito sao
atribuidos a threads diferentes enquanto forem maior que o limite minimo, caso
contrario, sao resolvidos com recursividade na mesma thread corrente.

Digite o codigo em C a seguir em um editor de texto e salve o arquivo com o
seguinte nome:

exemplo11.c

Depois, compile o programa em linha de comando da seguinte forma:

cc -lpthread exemplo11.c -o exemplo11  {ou use -pthread ao inves de -lpthread}

Corrija os erros de compilacao. Quando a compilacao for bem sucedida, o arquivo
executavel "exemplo11" serah criado.


E por fim execute da seguinte forma:

./exemplo11

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define maxVet 10000
#define maxNumero 10000

typedef  int tipoVetor[maxVet];

tipoVetor A;

typedef struct
{
    int ini, fim;
} tipoPack;

int tamA, limSegmento;

/* efetua a leitura do tamanho, considerando as restricoes
para valores negativos ou acima do limite maximo; retorna
no nome da funcao */

int leTam(char *titulo)
{   int tam;

    do
    {  printf("\nDigite o %s = ", titulo);
       scanf("%d", &tam);
    } while ((tam<1)||(tam>maxVet));

    return tam;
}

/* gera o vetor aleatoriamente, de acordo com o tamanho passado
   como parametro. O valores gerados podem ser positivos ou negativos
   e estao limitados a maxNumero. */

void geraVetor(tipoVetor V, int tam)
{   int i, sinal;

    srand(time(NULL));

    printf("\nGeracao Aleatoria do Vetor\n");

    for (i=0; i<tam; i++)
    {
        V[i]=rand()%maxNumero;
        sinal=(rand()%2)*2-1; /* se rand%2==0, sinal=-1;
                                 se rand%2==1, sinal=+1 */

        V[i]=sinal*V[i];  /* impoe o sinal no n�mero */
    }
}

/* mostra o vetor no video como uma sequencia de elementos
   separados por virgula */

void mostraVetor(char *id, tipoVetor V, int tam)
{   int i;

    printf("\nElementos do Vetor %s:\n", id);
    printf("%d",V[0]);
    for (i=1; i<tam; i++)
    {
        printf(", %d",V[i]);
    }
    printf("\n");
}

/*  junta/funde duas metades ja ordenadas dentro do segmento do
    vetor que vai de ini at� fim. A primeira metade vai de ini
    ate meio, a segunda vai de meio+1 ate fim. Usa-se 2 vetores
    auxiliares para copiar as metades, percorre-las, comparando
    os elementos e devolver para o segmento original, para que
    este fique totalmente ordenado. */

void mergeMetades(tipoVetor V, int ini, int meio, int fim)
{   tipoVetor auxV1, auxV2;
    int tam1=0, tam2=0, maior, i, i1, i2;

    /* copia primeira metade V[ini..meio] para auxV1 */
    for (i=ini; i<=meio; i++)
    {
        auxV1[tam1]=V[i];
        tam1++;
    }

    /* copia segunda metade V[meio+1..fim] para auxV2 */
    for (i=meio+1; i<=fim; i++)
    {
        auxV2[tam2]=V[i];
        tam2++;
    }

    /* descobre um valor maior que todos do segmento */

    if (auxV1[tam1-1]> auxV2[tam2-1])
        maior = abs(auxV1[tam1-1])+1;
    else maior = abs(auxV2[tam2-1])+1;

    /* insere o valor maior que todos logo apos o ultimo de
       cada metade, o qual servira como uma sentinela/barreira */

    auxV1[tam1]=maior;
    auxV2[tam2]=maior;

    /* avan�a em cada metade retirando o menor elemento e
       colocando de volta no segmento, para que todo o segmento
       fique ordenado */

    i1=0;
    i2=0;
    for (i=ini; i<=fim; i++)
    {
        if (auxV1[i1]<auxV2[i2])
        {
            V[i]=auxV1[i1];
            i1++;
        }
        else {
            V[i]=auxV2[i2];
            i2++;
        }
    }

}

/* solucao paralela e recursiva do Merge Sort para o segmento do vetor que vai
   de ini ate fim. Na primeira chamada, esse intervalo alcan�a todo o vetor.
   Em cada chamada, se houver pelo menos "limSegmento" elementos no segmento,
   o mesmo eh dividido em duas metades e para cada metade uma thread eh criada
   para fazer a mesma coisa, caso contrario, se houver mais do que 1 elemento no
   segmento, o mesmo eh dividido em duas metades e para cada metade, a funcao eh
   chamada de novo (recursivamente). A recursividade prossegue at� que surgem
   metades unitarias, quando ent�o ocorre o retorno da recursividade. No retorno,
   as metades unitarias sao juntadas (merge) e geram segmentos duplos, e da mesma
   forma, as metades duplas sao juntadas e geram  segmentos quadruplos, metades
   quadruplas geram segmentos octuplos e assim sucessivamente ate que todo o
   vetor esteja ordenado. */

void *mergeSort(void *ptr)
{   int meio, iretEsq, iretDir;
    tipoPack packEsq, packDir, *pack;
    pthread_t threadEsq, threadDir;

    pack=(tipoPack *)ptr;

    if (pack->fim-pack->ini>=limSegmento)
    {
        meio = (pack->ini+pack->fim)/2;

        packEsq.ini=pack->ini;
        packEsq.fim=meio;
        iretEsq = pthread_create(&threadEsq, NULL, mergeSort, (void*) &packEsq);

        packDir.ini=meio+1;
        packDir.fim=pack->fim;
        iretDir = pthread_create(&threadDir, NULL, mergeSort, (void*) &packDir);

        pthread_join(threadEsq, NULL);
        pthread_join(threadDir, NULL);

        mergeMetades(A,pack->ini,meio,pack->fim);
    }
    else if (pack->fim>pack->ini)
    {
        meio = (pack->ini+pack->fim)/2;

        packEsq.ini=pack->ini;
        packEsq.fim=meio;
        mergeSort((void *)&packEsq);

        packDir.ini=meio+1;
        packDir.fim=pack->fim;
        mergeSort((void*)&packDir);

        mergeMetades(A,pack->ini,meio,pack->fim);

    }

}

void main(void)
{ int resp;
  tipoPack pack;

  do
  {

    tamA=leTam("Tamanho do Vetor");

    limSegmento=leTam("Limite Minimo do Segmento");

    geraVetor(A, tamA);

    mostraVetor("A Original", A, tamA);

    printf("\nOrdenando A por Merge Sort ...\n");

    pack.ini=0;
    pack.fim=tamA-1;
    mergeSort((void*)&pack);

    mostraVetor("A Ordenado", A, tamA);

    printf("\nDeseja Nova Execucao, <1>Sim <2>Nao? => ");
    scanf("%d",&resp);

  } while (resp==1);
}
