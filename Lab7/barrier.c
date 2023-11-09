#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

int thread_no;
pthread_t* threads;

sem_t sem;
pthread_mutex_t mtx;

void barrier_point(){
    pthread_mutex_lock(&mtx);
    thread_no--;
    pthread_mutex_unlock(&mtx);
    if (thread_no == 0){
        sem_post(&sem);
        return;
    }
    sem_wait(&sem);
    sem_post(&sem);
    return;
}

void * tfun ( void * v ){
    int* tid = (int *) v;
    printf ("%d reached the barrier \n", *tid);
    barrier_point();
    printf ("%d passed the barrier \n", *tid);
    free (tid);
    return NULL;
}

void init(int n) {
    if (sem_init(&sem, 0, 0)){
        perror(NULL);
        return;
    }
    if (pthread_mutex_init(&mtx, NULL)){
        perror(NULL);
        return;
    }
    printf("NRTHRS=%d\n", n);

    thread_no = n;
    int cp_tn = thread_no;
    threads = (pthread_t *)malloc(cp_tn * sizeof(pthread_t));
    for (int i = 0; i < cp_tn; i++){
        int* id = malloc(sizeof(int));
        *id = i; 
        if (pthread_create(&threads[i], NULL, tfun, id)){
            perror(NULL);
            return;
        }
    }

    for (int i = 0; i < cp_tn; i++){
        if (pthread_join(threads[i], NULL)){
            perror(NULL);
            return;
        }
    }

    pthread_mutex_destroy(&mtx);
    sem_destroy(&sem);

}

int main(int argc, char* argv[]){

    init(10);
    return 0;
}