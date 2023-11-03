#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

void* reverse_str(void* string){
    char* str = (char *)string;
    //printf("%d", strlen(str));

    for (int i = 0; i < strlen(str)/2; i++){
        char aux = str[strlen(str) - i - 1];
        //printf("%c",aux);
        str[strlen(str) - i - 1] = str[i];
        str[i] = aux;
    }

    //str[sizeof(str) - 1] = '\0';
    //printf("%s",str);
    return (void*)str;
}

int main(int argc, char* argv[]){
    
    char* sir = malloc(sizeof(argv[1]));
    void* rasp = malloc(sizeof(argv[1]));
    strcpy(sir, argv[1]);
    //printf("%s\n", sir);

    pthread_t thr;
    if (pthread_create(&thr, NULL, reverse_str, sir)){
        perror(NULL);
        return errno;
    }

    if (pthread_join(thr, &rasp)){
        perror(NULL);
        return errno;
    }
    printf("%s\n", (char*)rasp);
    return 0;
}