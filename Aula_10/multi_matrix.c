#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 100

typedef struct {
    int row;
    int col;
    int (*A)[MAX];
    int (*B)[MAX];
    int (*C)[MAX];
    int n;
} ThreadData;

void* compute_element(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int sum = 0;
    for (int k = 0; k < data->n; ++k) {
        sum += data->A[data->row][k] * data->B[k][data->col];
    }
    data->C[data->row][data->col] = sum;
    pthread_exit(0);
}

void generate_matrix(int matrix[MAX][MAX], int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = rand() % 10;
        }
    }
}

void print_matrix(char* name, int matrix[MAX][MAX], int rows, int cols) {
    printf("%s:\n", name);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int A[MAX][MAX], B[MAX][MAX], C[MAX][MAX];
    int m, n, p;
    pthread_t threads[MAX][MAX];
    ThreadData thread_data[MAX][MAX];

    srand(time(NULL));

    printf("Digite o numero de linhas de A: ");
    scanf("%d", &m);
    printf("Digite o numero de colunas de A e linhas de B: ");
    scanf("%d", &n);
    printf("Digite o numero de colunas de B: ");
    scanf("%d", &p);

    generate_matrix(A, m, n);
    generate_matrix(B, n, p);

    print_matrix("Matriz A", A, m, n);
    print_matrix("Matriz B", B, n, p);

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < p; ++j) {
            thread_data[i][j].row = i;
            thread_data[i][j].col = j;
            thread_data[i][j].A = A;
            thread_data[i][j].B = B;
            thread_data[i][j].C = C;
            thread_data[i][j].n = n;
            pthread_create(&threads[i][j], NULL, compute_element, &thread_data[i][j]);
        }
    }

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < p; ++j) {
            pthread_join(threads[i][j], NULL);
        }
    }

    print_matrix("Matriz C (Resultado)", C, m, p);

    return 0;
}
