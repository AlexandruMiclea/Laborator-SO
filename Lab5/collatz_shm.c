#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define SHM_SZ (getpagesize() << 4)
#define WRITE_SZ (SHM_SZ >> 4)

int main(int argc, char* argv[]){
    
    //collatz(16);

    // allocate shared memory
    char shm_name[] = "collatz_shm";
    int shm_fd;

    shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0) {
        perror(NULL);
        return errno;
    }

    if (ftruncate(shm_fd, SHM_SZ) == -1){
        perror(NULL);
        shm_unlink(shm_name);
        return errno;
    }

    // load the shared memory into the process memory 
    // (we will do two pages, which we will divide into 16 equal parts)
    char* shm_ptr;
    shm_ptr = mmap(0, SHM_SZ, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if(shm_ptr == MAP_FAILED){
        perror(NULL);
        shm_unlink(shm_name);
        return errno;
    }

    // write the parent process id
    printf("Started parent %d\n", getpid());

    //printf("%s", "succesful up to here!\n");

    // create child processes for each argument
    // a child process can write in only one section of 
    // the memory!
    //printf("%d %d", shm_sz, write_sz);
    for (int i = 0; i < argc - 1; i++){
        pid_t pid = fork();
        if (pid < 0){
            printf("fail\n");
            perror(NULL);
            return errno;
        } else if (pid == 0){
            char* proc_ptr;
            // redo with mmap
            proc_ptr = mmap(0,WRITE_SZ,PROT_WRITE,MAP_SHARED, shm_fd,WRITE_SZ * i);
            if(proc_ptr == MAP_FAILED){
                perror(NULL);
                return errno;
            }
            int n = atoi(argv[i + 1]);
            //printf("%d\n", n);
            char* buf = malloc(WRITE_SZ);
            // sprintf
            char aux[10];
            sprintf(aux, "%d: ", n);
            strcat(buf, aux);
            //printf("here");
            while (n != 1) {
                //sprintf(buf, "%d ", n);
                aux[0] = '\0';
                sprintf(aux, "%d ", n);
                strcat(buf, aux);
                if (n % 2 == 0){
                    n = n / 2;
                } else {
                    n = 3 * n + 1;
                }
            };
            // printf("1.\n");
            strcat(buf, "1.\n");
            //sprintf(buf, "1.");

            // write the buffer to the memory
            //printf("%s\n",buf);
            //write(proc_ptr,buf,sizeof(buf));
            strcpy(proc_ptr, buf);

            // TODO call free on buf
            free(buf);
            munmap(proc_ptr, WRITE_SZ);

            // write that the process has finished execution
            printf("Done Parent %d Me %d\n", getppid(), getpid());

            // exit the process
            return 0;
        }
    }

    for (int i = 0; i < argc - 1; i++){
        wait(NULL);
    }

    // print the contents of the memory
    for (int i = 0; i < argc - 1; i++){
        printf("%s",shm_ptr + WRITE_SZ*i);
    }

    // print the parent process finish
    printf("Done Parent %d Me %d\n", getppid(), getpid());

    // unlink the shared memory file and
    // deallocate the memory zone used
    shm_unlink(shm_name);
    munmap(shm_ptr,SHM_SZ);

    return 0;
}