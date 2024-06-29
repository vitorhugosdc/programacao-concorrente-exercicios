/*

Universidade Estadual de Maringa
Departamento de Informatica

==================================================
Nome do Aluno:
RA:
Curso:
==================================================

Programacao Concorrente � Ano Letivo 2024

Prof. Ronaldo - Prova - Questao 01

20/06/2024 � das 13:30 as 15:20

Orientacoes Preliminares:

- Responda a questao diretamente neste arquivo. Ele ja contem o modelo basico de
  implementacao. Salve-o localmente.

- Preencha seu nome, RA e curso antes de iniciar a prova.

- Salve-o frequentemente para evitar perdas. Se voce perder o arquivo, nao sera
  dada outra oportunidade.

- Nao sera permitido acesso a internet. Desconecte o cabo na internet antes de
  iniciar a prova e reconecte novamente apenas quando for entregar, mas peca
  autorizacao antes.

- Nao sera permitido consulta a nenhum tipo de codigo, apenas ao documentos PDFs
  que o professor disponibilizar

- Quando terminar, informe o professor, reconecte a internet e envie-o arquivo
  .c na plataforma Moodle. Nao converta o arquivo em PDF ou em outro formato que
  nao seja este proprio.

- A implementacao deve ser feita em linguagem C, formatada e alinhada adequadamente.
  Desloque os comandos mais internos mais para a direita e alinhe as chaves
  correspondentes ao comando que pertencem, como o modelo a seguir:

		for (...)
		{
		    if (...)
		    {
			printf(...);
			gets(...);
			while (...)
			{
			    strcmp(...);
			    a[i]=a[i+1];
			}
		    }
		    scanf(...);
		    x=soma(...);
		}

- use nomes de variaveis adequadas a sua finalidade, por exemplo, "soma", "nome",
  "idade" etc. e nao use nomes que nada significam, tipo "w_p3", "xyz", "pa_daqui"
  etc.

- nao declare e nem use variaveis auxiliares/secundarias desnecessarias, que nao
  causam efeito relevante no programa.

ALERTA: Nao obtenha respostas de terceiros, senao sua prova sera anulada.

ATENCAO: Para avaliar sua resposta, a implementacao sera compilada e executada pelo
professor. Voce pode usar o codeblocks para implementar. Sua implementacao devera
funcionar.

Boa Prova!

================================================

Questao 01 (2,5 pontos):

Implemente uma aplicacao paralela/concorrente multi-threaded, usando linguagem C
com pthreads. Nao use recursos de orientacao a objetos, apenas linguagem C padrao
com as bibliotecas constante no modelo basico. Nao use outras bibliotecas. Nao use
comandos de threads que nao foram trabalhados em sala. Implemente uma aplicacao
completa, com todas as funcoes que forem necessarias, para que ela funcione de
forma segura, sem erros, ainda que raros. Desenvolva por si soh toda a codificacao
que for necessaria. Se estas instru��es nao forem seguidas, sua resposta sera
anulada/zerada.

A aplicacao deve localizar um vetor menor de tamanho M dentro de outro vetor
maior de tamanho N, sendo N>=M. Obtenha M e N do usuario. Gere aleatoriamente
os elementos inteiros do vetor maior. Mostre na tela o vetor maior que foi gerado.
Depois, os elementos do vetor menor devem ser fornecidos pelo usuario. Mostre na
tela o vetor menor que foi fornecido. Os vetores devem ser mostrados horizontalmente
com os elementos separados por virgula e/ou espaco.

A aplicacao deve encontrar todas as correpondencias do vetor menor dentro do
vetor maior. Uma correspondencia eh encontrada quando todos os elementos do vetor
menor se encontram, a partir de determinada posicao, na mesma ordem e sentido,
dentro do vetor maior, elemento a elemento. Ao final, a aplicacao devera mostrar
o numero total de correspondencias e as respectivas posicoes iniciais onde foram
encontradas. Use K threads para resolver este problema, devendo K ser obtido do
usuario.

Cada thread deve se responsabilizar por uma parte do vetor maior e deve tentar achar
as correspondencias que se INICIAM dentro da sua parte apenas. Um correspondencia
pode iniciar dentro de uma parte e terminar em outra, sem problema. Assim, as
threads podem acabar percorrendo as partes seguintes de responsabilidade de outras
threads, na tentiva de procurar a correspondencia que se iniciou na sua parte.


Boa prova!

DICAS DE COMANDOS E MODELO BASICO

SOBRE THREADS:

    pthread_t thread;

    iret = pthread_create(&thread, NULL, funcao_da_thread, (void*) &pack);

    pthread_join(thread, NULL);


SOBRE TEMPO:

    time_t tempoSeg;

    struct tm *tempoLocal;

    time(&tempoSeg);

    tempoLocal=localtime(&tempoSeg);

    asctime(tempoLocal);


SOBRE ALEATORIO:

    srand(time(NULL));

    rand()


*/

/* IMPLEMENTE A PARTIR DAQUI */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define TAM_MAX 50

void *funcao_da_thread(void *ptr);

typedef struct {
    int id;
    int inicio;
    int fim;
    int *M;
    int *N;
} thread_args;

int posicoes[TAM_MAX];
int count = 0;
int TAM_M;
int TAM_N;

int main() {
    printf("Tamanho Vetor N: ");
    scanf("%d", &TAM_N);
    int N[TAM_N];
    
    printf("Tamanho Vetor M: ");
    scanf("%d", &TAM_M);
    int M[TAM_M];

    printf("Threads: ");
    int num_threads;
    scanf("%d", &num_threads);

    pthread_t threads[num_threads];
    thread_args args[num_threads];

    srand(time(NULL));
    printf("Vetor gerado N: ");
    for (int i = 0; i < TAM_N; i++) {
        N[i] = rand() % 10;
        printf("%d ", N[i]);
    }

    printf("\nInsira vetor M: ");
    for (int i = 0; i < TAM_M; i++) {
        scanf("%d", &M[i]);
    }
    printf("Vetor entrado M: ");
    for (int i = 0; i < TAM_M; i++) {
        printf("%d ", M[i]);
    }
    
    printf("\n");

    int divisao = TAM_N / num_threads;

    for (int i = 0; i < num_threads; i++) {
        args[i].id = i;
        args[i].inicio = i * divisao;
        args[i].fim = (i == num_threads - 1) ? TAM_N - TAM_M + 1 : (i + 1) * divisao;
        args[i].M = M;
        args[i].N = N;
        
        pthread_create(&threads[i], NULL, funcao_da_thread, &args[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Total de correspondencias: %d\n", count);
    printf("Posicoes encontradas: ");
    for (int i = 0; i < count; i++) {
        printf("%d ", posicoes[i]);
    }
    printf("\n");

    return 0;
}

void *funcao_da_thread(void *ptr) {
    thread_args *args = (thread_args *)ptr;
    for (int i = args->inicio; i < args->fim; i++) {
        int match = 1;
        for (int j = 0; j < TAM_M; j++) {
            if (args->N[i + j] != args->M[j]) {
                match = 0;
                break;
            }
        }
        if (match) {
            posicoes[count++] = i;
        }
    }
}
