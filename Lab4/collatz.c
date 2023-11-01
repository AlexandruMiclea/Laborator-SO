#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>

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
    pid_t pid = fork();
    if (pid == 0){
        // child process
        collatz(atoi(argv[1]));
        printf("Done Parent %d Me %d.\n",getppid(),getpid());
    }
    
    return 0;
}