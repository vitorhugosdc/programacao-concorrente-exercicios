/* Aplica��o de Base: Calcula Produto Escalar utilizando programa��o convencional
(sequencial). No produto escalar, um vetor v1 eh multiplicado por um vetor v2,
por meio da multiplicacao de elemento a elemento, somando-se os resultados das
multiplicacoes parciais, gerando um unico valor numerico. Matematicamaente, temos
que ProdEsc=Somatoria de V1[i]*V2[i], para i=0 ateh i=n-1, para vetores V1 e V2
com n elementos.

Digite o codigo em C a seguir em um editor de texto e salve o arquivo com o
seguinte nome:

prodEscalar.c

Depois, compile o programa em linha de comando da seguinte forma:

cc prodEscalar.c -o prodEscalar

E por fim execute da seguinte forma:

./prodEscalar

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define maxVetor 100

typedef int tipoVetor[maxVetor];

tipoVetor v1, v2;

int nElem;

int produtoEscalar(tipoVetor A, tipoVetor B, int n)
{   int PE, i;

    PE = 0;
    for (i=0; i<n; i++)
        PE=PE+A[i]*B[i];

    return (PE);
}

void geraVetor(char *tipo, tipoVetor V, int n)
{   int i;

    printf("\n%s: ", tipo);
    for (i=0; i<n; i++)
    {   V[i]=rand()%10;
        printf("%d, ",V[i]);
    }
    printf("\n");

}


void main(void)
{   int PE;

    srand(time(NULL));

    printf("\nQuantos elementos tem os vetores? => ");
    scanf("%d",&nElem);

    geraVetor("Vetor V1",v1, nElem);
    geraVetor("Vetor V2",v2, nElem);

    PE=produtoEscalar(v1, v2, nElem);

    printf("\n\nProduto Escalar = %d\n", PE);

}


/* Desafio: ajustar este codigo para calcular multiplicacao matricial com multi-
plas threads, sendo Cmxp=Amxn*Bnxp. As matrizes A e B devem ser geradas aleatori-
amente e a matriz C deve ser calculada. Todas as matrizes devem ser mostradas no
video. Utilize uma thread para cada produto escalar linhaxcoluna */