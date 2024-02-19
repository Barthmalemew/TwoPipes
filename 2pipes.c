#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>

#define buffer_size 25
int main(){ 
    
    //creates a c style string(array of char) with a predefined buffersize
    char msg[buffer_size];
    
    //creates pipe one and two for later use
    int p1[2];
    int p2[2];

    pid_t p;

    //checkes the created pipes for failure. must be done before forking
    if(pipe(p1) == -1 || pipe(p2) == -1)
    {
        fprintf(stderr, "pipe failed");
    }

    //you MUST fork here or the fork will not be created.
    //Creates a parent and child process.
    p = fork();

    //checks for fork failure
    if(p < 0)
    {
        fprintf(stderr, "fork failed");
    }

    //parent
    else if(p > 0)
    {
        //pipe 1
        //close unused read end
        close(p1[0]);

        scanf("%s", msg);

        write(p1[1], msg, strlen(msg) + 1);

        close(p1[1]);

        wait(NULL);

        //pipe 2
        //close unused write end
        close(p2[1]);

        read(p2[0], msg, buffer_size);

        printf("case swap made: %s\n", msg);

        close(p2[0]);
    }

    //child
    else
    {
        //pipe 1
        //close unused write end
        close(p1[1]);
        
        read(p1[0], msg, buffer_size);
        printf("child recieved message: %s\n", msg);

        //changes the cases of all letters sent to user until a whitespace.
        for(int i = 0; i < strlen(msg); i++)
        {
            if(isupper(msg[i]))
                msg[i] = tolower(msg[i]);
            else
                msg[i] = toupper(msg[i]);
        }

        close(p1[0]);

        //pipe 2
        //close unused read end
        close(p2[0]);

        write(p2[1], msg, strlen(msg) + 1);

        close(p2[1]);

    }

    return 0;
}
