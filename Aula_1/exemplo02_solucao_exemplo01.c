/*

SOLUÇÃO DOS DESAFIOS DO EXEMPLO 01 (feito do professor)

Exemplo 02: implementa os desafios do Exemplo 01. Cria 3 threads e insere modificacoes.

===========
Explicacoes
===========

O prototipo da funcao de criacao de uma thread eh especificado da seguinte forma.

int pthread_create(pthread_t *thread,
                   const pthread_attr_t *attr,
                   void *start_routine(void *),
                   void *arg);

Ela usa 4 parametros:

1) O primeiro, "thread", eh o ponteiro que contem o endereco de uma estrutura do tipo "pthread_t" (tambem chamado de descritor da thread). Essa estrutura deve ser declarada no programa principal (ou processo; aquele que cria as threads).

2) O segundo, "attr", eh o ponteiro que contem o endereco de uma estrutura do tipo "pthread_attr_t", que contem atributos iniciais para a thread. Essa estrutura deve ser declarada no programa principal ou NULL pode ser usado para indicar atributos padroes.

3) O terceiro, "start_routine", eh o nome da funcao que sera executada pela thread criada. Essa funcao deve ser especificada como sendo do tipo ponteiro para void (void*).

4) O quarto e ultimo parametro, "arg", eh o ponteiro que contem o endereco do unico parametro a ser passado para a funcao "start_routine". Esse parametro pode representar um tipo simples, string, vetorial, estrutura ou area de memoria, declarado no programa principal, mas deve ser convertido como ponteiro para void (void*).

Se bem sucedida, a funcao pthread_create() retorna 0, caso contrario, retorna o numero do erro. Nesse caso, o valor retornado no argumento "thread" eh indefinido.

Depois que as threads sao criadas, o programa principal tambem pode ser referenciado como a thread principal.

A espera pelo termino das threads pode ser feito pela funcao pthread_join que possui o seguinte prototipo:

int pthread_join(pthread_t thread, void **retval);

Ela usa 2 parametros:

1) O primeiro, "thread", identifica o descritor de uma thread que tenha sido criada de forma bem sucedida.
2) O segundo, "retval", retorna o status de termino da thread, exceto se NULL for utilizado.

Se bem sucedida, a funcao pthread_join() retorna 0, caso contrario, retorna o numero do erro. Nesse caso, o valor retornado no argumento "retval" eh indefinido.

===========
Experimento
===========

Digite o codigo em C a seguir em um editor de texto e salve o arquivo com o seguinte nome:

exemplo02.c

Depois, compile o programa em linha de comando da seguinte forma:

cc -lpthread exemplo02.c -o exemplo02

ATENCAO: dependendo da distribuicao Linux, eh possivel que a biblioteca de threads tenha outra forma de chamada, como por exemplo, pthread, sem o 'l' de library. Nesse caso, use apenas -pthread ao inves de  -lpthread.

Corrija os erros de compilacao. Quando a compilacao for bem sucedida, o arquivo executavel "exemplo02" serah criado.

Entao, execute o executavel com a seguinte linha de comando:

./exemplo02

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int v[3000];

void *funcaoImprimeMensagem( void *ptr );

void main(void)
{
    pthread_t thread1, thread2, thread3;
    char *mensagem1 = "\nSou a thread 1";
    char *mensagem2 = "\nSou a thread 2";
    char *mensagem3 = "\nSou a thread 3";
    int  i, iret1, iret2, iret3;

    /* Cria 3 threads independentes e ambas executarao a mesma funcao
       com argumentos diferentes */

    iret1 = pthread_create(&thread1, NULL, funcaoImprimeMensagem, (void*) mensagem1);

    iret2 = pthread_create(&thread2, NULL, funcaoImprimeMensagem, (void*) mensagem2);

    iret3 = pthread_create(&thread3, NULL, funcaoImprimeMensagem, (void*) mensagem3);

    printf("\nCriacao da thread 1 retornou: %d\n",iret1);
    printf("\nCriacao da thread 2 retornou: %d\n",iret2);
    printf("\nCriacao da thread 3 retornou: %d\n",iret3);

    /* Espera as threads completarem para entao continuar, para nao
       correr o risco de terminar o programa principal antes das
       threads terminarem. O termino do processo causa forcadamente
       o termino de suas tarefas */

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    printf("\nAmbas as threads 1, 2 e 3 ja finalizaram e agora eu finalizarei...\n");

    for (i=0; i<3000; i++)
        printf("v[%d] = %d\n",i, v[i]);

    exit(0);
}

void *funcaoImprimeMensagem(void *ptr )
{   int i, nt;
    char *mensagem;

    mensagem = (char *) ptr;
    nt=atoi(&(mensagem[14]));

     for (i=0; i<1000; i++)
     {
        printf("%d > %s \n", i, mensagem);
        v[i+((nt-1)*1000)]=nt;
     }
}



