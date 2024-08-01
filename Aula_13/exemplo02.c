/*

Exemplo 02: Testes com a identificacao de processos no OPENMPI.

Salve este arquivo com o nome exemplo02.c

Compile-o em um terminal pela linha de comando, da seguinte forma:

>mpicc exemplo02.c -o exemplo02 <enter>

Depois execute-o pela linha de comando, da seguinte forma:

>mpirun -np 5 ./exemplo02 bla ble bli blo blu <enter>

Atencao: talvez seja necessario utilizar a opcao --oversubscribe na
linha de comando do mpirun para permitir executar mais processos doque
o numero de cores/processadores.

Analise o codigo e a execucao. Teste diferentes numeros de processos.

*/

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]){
    int id, np, i;

    int vect[1000];

    for(i = 0;i < 1000; i++){
        vect[i] = i;
    }

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&id);
    MPI_Comm_size(MPI_COMM_WORLD,&np);

    int tamanho = 1000 / np;
    int resto = 1000 % np;

    int inicio = id * tamanho + (id < resto ? id : resto);
    int fim = inicio + tamanho + (id < resto ? 1 : 0);

    double sum = 0.0;

    for(i = inicio; i < fim; i++){
       sum += vect[i];
    }

    tamanho = fim - inicio;
    printf("\nProcesso %d, Intervalo [%d, %d], Media = %.2f\n", id, inicio, fim, sum / tamanho);

    MPI_Finalize();
}

/* Desafio: Faca um programa que declare e inicialize estaticamente um
vetor de 1000 elementos e divida esse vetor equilibradamente entre os
processos, de forma que cada processo ira calcular a media aritmetica
de sua parte. Cada processo deve imprimir na tela a sua identificacao
(id ou rank), o seu intervalo dos dados, a sua parte do vetor e a media
final */