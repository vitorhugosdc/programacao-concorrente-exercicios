/*

Exemplo 03: testar a passagem de uma informacao composta para as threads, permitindo
 o retorno de calculos realizados por elas. Nesse exemplo, duas threads realizam
 calculo de soma de numeros sequenciais.

===========
Experimento
===========

Digite o codigo em C a seguir em um editor de texto e salve o arquivo com o seguinte
nome:

exemplo03.c

Depois, compile o programa em linha de comando da seguinte forma:

cc -lpthread exemplo03.c -o exemplo03  {ou use -pthread ao inves de -lpthread}

Corrija os erros de compilacao. Quando a compilacao for bem sucedida, o arquivo
executavel "exemplo03" serah criado.

Entao, execute o executavel com a seguinte linha de comando:

./exemplo03

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define NUM_THREADS 2

int flag = 0;
int valor;
int fibonacci = 0;

void *funcaoSomaSequencial(void *ptr);
void *funcaoFibonacci(void *ptr);

struct tipoPack {
   char mensagem[16];
   int tam, soma;
};

typedef struct{
     int thread_id, resultado;
}thread_args;

typedef struct tipoPack tipoPack;

int main(void)
{
     pthread_t thread1, thread2;

     int iret1, iret2;

     tipoPack pack1, pack2;

     strcpy(pack1.mensagem,"\nSou a thread 1");
     pack1.soma=0;

     strcpy(pack2.mensagem,"\nSou a thread 2");
     pack2.soma=0;

     printf("\nDigite o tamanho da sequencia 1 > ");
     scanf("%d",&(pack1.tam));

     printf("\nDigite o tamanho da sequencia 2 > ");
     scanf("%d",&(pack2.tam));

     iret1 = pthread_create(&thread1, NULL, funcaoSomaSequencial, (void*) &pack1);

     iret2 = pthread_create(&thread2, NULL, funcaoSomaSequencial, (void*) &pack2);

     pthread_join(thread1, NULL);
     pthread_join(thread2, NULL);

     printf("\nThread 1 somou: %d\n",pack1.soma);
     printf("\nThread 2 somou: %d\n",pack2.soma);


     /*----------DESAFIO 2----------*/

    pthread_t threads[NUM_THREADS];
    thread_args args[NUM_THREADS];
    flag = 0;
    int i, n;
    printf("\nDigite o n-esimo numero de Fibonacci a ser calculado: > ");
    scanf("%d", &n);

    if (n == 0) {
        printf("Fibonacci de 0 eh 0\n");
        return 0;
    } else if (n == 1) {
        printf("Fibonacci de 1 eh 1\n");
        return 0;
    }

    fibonacci = 1;
    for (i = 0; i < NUM_THREADS; i++) {
        valor = n;
        args[i].thread_id = i;
        pthread_create(&threads[i], NULL, funcaoFibonacci, (void*)&args[i]);
    }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Fibonacci de %d eh %d\n", n, fibonacci);

     exit(0);
}

void *funcaoSomaSequencial(void *ptr)
{    int i;

     tipoPack *pack;

     pack= (tipoPack *) ptr;

     printf("%s: vou realizar uma somatoria", pack->mensagem);

     for (i=1; i<=(pack->tam); i++)
	     (pack->soma) = (pack->soma) + i;
     return NULL;
}

void *funcaoFibonacci(void *ptr) {
    thread_args *args = (thread_args*) ptr;
    int a = 0, b = 1, temp;

    for (int i = 2; i <= valor; i++) {
        while (flag != args->thread_id);
        temp = a + b;
        a = b;
        b = temp;
        flag = 1 - flag;
    }

    while (flag != args->thread_id);
    if (args->thread_id == 1) {
        fibonacci = b;
    }
    flag = 1 - flag;

    return NULL;
}

/*

Desafios para Entregar HOJE:

1) Primeira Implementacao: Modifique o exemplo para que as threads calculem e
retornem a fatorial de um numero. Ajuste os nomes das variaveis. Cada thread deve
calcular uma fatorial diferente, conforme informado pelo usuario. Compile, execute
e analise.

2) Segunda Implementacao: Modifique o exemplo para que as threads calculem e
retornem o n-esimo elemento da sequencia de fibonacci, conforme n fornecido pelo
usuario. As threads devem trabalhar em conjunto para calcular esse valor. Crie
uma solu��o da melhor forma que voce puder. Ajuste os nomes das variaveis.
Compile, execute e analise.
*/

