#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#define MAX_RESOURCES 10
int available_resources = MAX_RESOURCES;

pthread_mutex_t mtx;
pthread_t* threads;

int decrease_count(int count){
    pthread_mutex_lock(&mtx);
    if (available_resources < count){
        //printf("ERROR: Tried to get %d resources %d remaining\n", count, available_resources);
        pthread_mutex_unlock(&mtx);
        return -1;
    }
    else {
        available_resources -= count;
        printf("Got %d resources %d remaining\n", count, available_resources);
        pthread_mutex_unlock(&mtx);
    }
    return 0;
}

int increase_count(int count){
    pthread_mutex_lock(&mtx);
    available_resources += count;
    printf("Released %d resources %d remaining\n", count, available_resources);
    pthread_mutex_unlock(&mtx);
    return 0;
}

void* reserve(void* arg){
    int val = *(int*)arg;

    int aux = -1;
    while (aux != 0){
        aux = decrease_count(val);
    }
    sleep(1);
    increase_count(val);
    return NULL;
}

int main(int argc, char* argv[]){

    if (pthread_mutex_init(&mtx, NULL)){
        perror(NULL);
        return errno;
    }

    threads = (pthread_t *)malloc(MAX_RESOURCES * sizeof(pthread_t));
    for (int i = 0; i < MAX_RESOURCES; i++){
        // random resource value - include math lib
        int rnd = rand() % MAX_RESOURCES;
        if (pthread_create(&threads[i], NULL, reserve, &rnd)){
            perror(NULL);
            return errno;
        }
    }

    for (int i = 0; i < MAX_RESOURCES; i++){
        if (pthread_join(threads[i], NULL)){
            perror(NULL);
            return errno;
        }
    }

    pthread_mutex_destroy(&mtx);

    return 0;
}