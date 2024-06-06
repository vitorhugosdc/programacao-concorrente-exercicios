/*

Exemplo 06: implementa��o do desafio do exemplo 04 - fatorial com k threads,
sendo que cada thread calcula um fatorial diferente

===========
Experimento
===========

Digite o codigo em C a seguir em um editor de texto e salve o arquivo com o seguinte
nome:

exemplo06.c

Depois, compile o programa em linha de comando da seguinte forma:

cc -lpthread exemplo06.c -o exemplo06  {ou use -pthread ao inves de -lpthread}

Corrija os erros de compilacao. Quando a compilacao for bem sucedida, o arquivo
executavel "exemplo06" serah criado.

Entao, execute o executavel com a seguinte linha de comando:

./exemplo06

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int NUM_THREADS;
int N;

void *funcaoCalculaFatorial(void *ptr);

struct tipoPack {
   int id;
   char mensagem[16];
   double fat;
   int start;
   int end;
};

typedef struct tipoPack tipoPack;

int main(int argc, char *argv[])
{
   if(argc != 3 || atoi(argv[1]) < 1 || atoi(argv[2]) < 1){
      printf("Uso: %s <fatorial> <threads>\n", argv[0]);
      return 1;
   }

   N = atoi(argv[1]);
   NUM_THREADS = atoi(argv[2]);

   pthread_t thread[NUM_THREADS];
   tipoPack pack[NUM_THREADS];

   int intervalo = N / NUM_THREADS;
   int resto = N % NUM_THREADS;

   for (int i = 0; i < NUM_THREADS; i++) {
         sprintf(pack[i].mensagem, "\nSou a thread %d", i);
         pack[i].fat = 1.0;
         pack[i].id = i;
         pack[i].start = i * intervalo + 1;
         pack[i].end = (i + 1) * intervalo;
   }

   for (int i = 0; i < resto; i++) {
         pack[i].end++;
         for (int j = i + 1; j < NUM_THREADS; j++) {
             pack[j].start++;
             pack[j].end++;
         }
   }

   for (int i = 0; i < NUM_THREADS; i++)
      pthread_create(&(thread[i]), NULL, funcaoCalculaFatorial, (void*) &pack[i]);

   for (int i = 0; i < NUM_THREADS; i++)
      pthread_join(thread[i], NULL);

   for (int i = 0; i < NUM_THREADS; i++)
      printf("\nThread %d fatoriou de %d a %d: %lf\n", i, pack[i].start, pack[i].end, pack[i].fat);

   double result = 1.0;

   for (int i = 0; i < NUM_THREADS; i++) {
      result *= pack[i].fat;
   }
   printf("\nResultado: %.2f\n", result);

   return 0;
}

void *funcaoCalculaFatorial(void *ptr)
{
   tipoPack *pack = (tipoPack *) ptr;

   printf("%s: vou calcular a fatorial de %d a %d\n", pack->mensagem, pack->start, pack->end);

   for(int i = pack->start; i <= pack->end; i++){
      pack->fat *= i;
   }
   return NULL;
}

/* Desafio: modifique o codigo para calcular tofarial com k threads.

Oque chamamos aqui de tofarial � uma fun��o que tenta imitar uma fatorial distribuida
entre k threads. Cada thread calculara o seu intervalo de fatorial e no final, a
thread main() ira calcula a "tofarial" total por meio de uma multiplicacao de todas as
fatoriais inidividuais. Matematicamente, esse metodo n�o calcula a fatorial de
forma correta e por isso chamados de tofarial.
*/