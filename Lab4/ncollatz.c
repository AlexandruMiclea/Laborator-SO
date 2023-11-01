#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <math.h>

// collatz: n/2,        n % 2 == 0
//          3n + 1,     n % 2 == 1

void collatz(int n){
    printf("%d: %d ",n,n);
    do {
        if (n % 2 == 0){
            n = n / 2;
        } else {
            n = 3 * n + 1;
        }
        printf("%d ",n);
    } while (n != 1);
    printf(".\n");
}

int main(int argc, char* argv[], char* envp[]) {
    printf("starting parent %d\n", getpid());
    for (int i = 1; i <= argc; i++){
        pid_t pid = fork();
        if (pid == 0){
            collatz(atoi(argv[i]));
            printf("done me: %d, dad: %d\n",getpid(),getppid());
            return 0;
        } 
    }
    for (int i = 1; i <= argc; i++){
        wait(NULL);
    }
    printf("done me: %d, dad: %d\n",getpid(),getppid());
    return 0;
}