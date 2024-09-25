#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 100

int main(int argc, char *argv[]) {

    int thread_count = strtol(argv[1], NULL, 10);

    int i, j, k;
    double sum;
    double start, end;
    double A[N][N], B[N][N], C[N][N];
    double C_parallel[N][N];
    
    // 初始化矩阵
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = j * 1;
            B[i][j] = i * j + 2;
            C[i][j] = j - i * 2;
            C_parallel[i][j] = j - i * 2;
        }
    }

    // 串行版本
    start = omp_get_wtime(); // 开始时间
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            sum = 0;
            for (k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
    end = omp_get_wtime(); // 结束时间
    printf("%f,", end - start);

    // 并行版本
    start = omp_get_wtime(); // 开始时间

    #pragma omp parallel for num_threads(thread_count) private(i, j, k, sum) shared(A, B, C_parallel)
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            sum = 0;
            for (k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            C_parallel[i][j] = sum;
        }
    }

    end = omp_get_wtime(); // 结束时间
    printf("%f", end - start);

    return 0;
}