#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define MSGSIZE 25
char* msg = "CHILD: value = ";

int value = 5;
char valueStr[5];
char pipeBuf[MSGSIZE];
int nbytes;

int main(){
    pid_t pid;
    int p[2], nbytes;
    pipe(p);
    pid = fork();

    
    if(pid == 0){ //Child process
        value += 15;
        snprintf( pipeBuf, MSGSIZE, "%d", value );
        close(p[0]);
        write(p[1], pipeBuf, MSGSIZE);
        close(p[1]);
    }else if(pid > 0){ //Parent process
        wait(NULL);
        printf("PARENT: value = %d\n",value); /* LINE A */
        close(p[1]);
        nbytes = read(p[0], pipeBuf, MSGSIZE);
        printf("%s %s\n",msg, pipeBuf); /* LINE B */
        close(p[0]);
    }
    return 0;
}