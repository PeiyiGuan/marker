#include<stdio.h>
#include <stdlib.h>
#include<unistd.h>

int main(int argc, char *argv){
    
    printf("execDemo: my pid is %d\n",getpid());

    // with L : comma seperated arguments
    // with V: vector(ie :: array of strings)
    char *args[] = {"helloExec.c","hello","world",NULL};

    execv("ls",args);
}