#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>

/*Scrieti un program mycp care s˘a primeasc˘a la intrare ˆın
primul argument un fis, ier surs˘a pe care s˘a-l copieze
 ˆıntr-un alt fis, ier cu numele primit ˆın al
doilea argument. Exemplu apel: ./mycp foo bar*/

int main(int argc, char* argv[]){
    struct stat s1;
    stat(argv[1], &s1);
    char data[s1.st_size];
    //char* data = malloc(s1.st_size * sizeof(char));
    printf("%ld", s1.st_size * sizeof(char));
    chmod(argv[1], S_IRWXU);
    int loc = open(argv[1], O_RDONLY);
    //printf("%d", loc);
    read(loc, (void*)data, (size_t)s1.st_size);
    close(loc);
    //printf("%s", data);
    //printf("%d", s1.st_size);
    int loc2 = creat(argv[2], O_CREAT);
    chmod(argv[2], S_IRWXU);
    open(argv[2], O_RDWR);
    write(loc2, (void*)data, sizeof(data));
    close(loc2);
}