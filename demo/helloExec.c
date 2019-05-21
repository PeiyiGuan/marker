#include<stdio.h>
#include<unistd.h>

int main(int argc,char *argv[]){

 //   printf("my argv[0] is %s \n",argv[0]);
    printf("runing helloExec:\n");

    printf("my PID %d \n",getpid());
    printf("my arguments: ");

    for(int i=0;i<argc;i++){
        printf("%s  ",argv[i]);
    }
    printf("\n");

    return 0;
}