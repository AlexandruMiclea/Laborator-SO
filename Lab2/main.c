#include <unistd.h>
//#include <stdio.h>

int main() {
    ssize_t out = write(1, "Hello world!\n",13);
    //printf("%d", (void*)out);
    //return 0;
}