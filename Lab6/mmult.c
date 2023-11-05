#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

int **m1, **m2, **ans;
int m, n, p, cnt = 0;
int **garbage;

typedef struct {
    int i, j;
} params;

void* mult(void* arg){
    int* anss = malloc(sizeof(int));
    garbage[cnt++] = anss;
    //int anss = 0;
    params* actual = (params*)arg;
    int lin = actual->i;
    int col = actual->j;

    for(int k = 0; k < n; k++){
        *anss += m1[lin][k] * m2[k][col];
    }    

    free(actual);
    //printf("%d", *anss);
    return anss;
}

int main(int argc, char* argv[]){

    // read values
    pthread_t threads[110];
    printf("Matricea 1 va fi de forma m*n, a doua de forma n*p.\n");
    printf("m = ");
    scanf("%d", &m);
    printf("n = ");
    scanf("%d", &n);
    printf("p = ");
    scanf("%d", &p);

    // malloc the matrixes
    m1 = (int**)malloc(m * sizeof(int*));
    m2 = (int**)malloc(n * sizeof(int*));
    ans = (int**)malloc(m * sizeof(int*));
    garbage = (int**)malloc(m*p * sizeof(int*));
    if (m1 == NULL || m2 == NULL || ans == NULL){
        printf("Malloc failed!");
        return errno;
    }

    for (int i = 0; i < n; i++){
        m1[i] = (int*)calloc(n, sizeof(int));
        if (m1[i] == NULL){
            printf("Calloc failed!\n");
            return errno;
        }
    }
    for (int i = 0; i < p; i++){
        m2[i] = (int*)calloc(n, sizeof(int));
        if (m2[i] == NULL){
            printf("Calloc failed!\n");
            return errno;
        }
    }
    for (int i = 0; i < p; i++){
        ans[i] = (int*)calloc(n, sizeof(int));
        if (ans[i] == NULL){
            printf("Calloc failed!\n");
            return errno;
        }
    }

    // read the matrixes
    printf("Valorile pentru m1 (%d*%d): \n", m, n);
    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
            scanf("%d", &m1[i][j]);
        }
    }

    printf("Valorile pentru m2 (%d*%d): \n", n, p);
    for (int i = 0; i < n; i++){
        for (int j = 0; j < p; j++){
            scanf("%d", &m2[i][j]);
        }
    }

    // create new threads for processing the matrix multiplication
    for (int i = 0; i < m; i++){
        for (int j = 0; j < p; j++){
            params* arg = malloc(sizeof(arg));
            arg->i = i;
            arg->j = j;
            if (pthread_create(&threads[i*m + j], NULL, mult, arg)) {
                perror(NULL);
                return errno;
            }
        }
    }

    for (int i = 0; i < m; i++){
        for (int j = 0; j < p; j++){
            int* res;
            if (pthread_join(threads[i*m + j], (void*)&res)) {
                perror(NULL);
                return errno;
            }
            ans[i][j] = *res;
        }
    }

    // print the answer matrix
    printf("The ans matrix (%d*%d) is: \n", m,p);
    for (int i = 0; i < m; i++){
        for (int j = 0; j < p; j++){
            printf("%d ", ans[i][j]);
        }
        printf("\n");
    }

    // TODO free the matrixes

    for (int i = 0; i < m; i++){
        free(m1[i]);
        free(ans[i]);
    }
    for (int i = 0; i < n; i++){
        free(m2[i]);
    }
    for (int i= 0; i < m * p; i++) free(garbage[i]);
    free(garbage);
    free(m1);
    free(m2);
    free(ans);

    return 0;
}