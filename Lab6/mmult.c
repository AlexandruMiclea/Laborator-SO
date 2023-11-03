#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

int m1[5][5], m2[5][5], ans[5][5];

typedef struct {
    int i, j;
} params;

void* mult(void* arg){
    int* anss = malloc(sizeof(int));
    params* actual = (params*)arg;
    int lin = actual->i;
    int col = actual->j;

    for(int k = 0; k < 5; k++){
        *anss += m1[lin][k] * m2[k][col];
    }    

    free(actual);
    //printf("%d", *anss);
    return anss;
}

int main(int argc, char* argv[]){

    pthread_t threads[30];    

    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++){
            m1[i][j] = 1;
            m2[i][j] = 2;
        }
    }

    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++){
            params* arg = malloc(sizeof(arg));
            arg->i = i;
            arg->j = j;
            if (pthread_create(&threads[i*5 + j], NULL, mult, arg)) {
                perror(NULL);
                return errno;
            }
        }
    }

    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++){
            int* res;
            if (pthread_join(threads[i*5 + j], (void*)&res)) {
                perror(NULL);
                return errno;
            }
            ans[i][j] = *res;
        }
    }

    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++){
            printf("%d ", ans[i][j]);
        }
        printf("\n");
    }


    return 0;
}