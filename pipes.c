#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main() {
    char write_msg[BUFFER_SIZE]; // for user input
    char read_msg[BUFFER_SIZE]; // for the resulting case swapped message

    int p1[2]; // pipe 1
    int p2[2]; // pipe 2

    pid_t pid; // fork

    // If either pipe fails, close program
    if(pipe(p1) == -1 || pipe(p2) == -1) {
        printf("Pipe failed\n");
        return 1;
    }

    // Fork
    pid = fork();

    // If fork failed
    if (pid < 0)
    {
        printf("Fork failed\n");
        return 1;
    }
    // Parent process (writes)
    else if (pid > 0)
    {
        // Close the read end of p1 and the write end of pipe 2
        close(p1[READ_END]);

        close(p2[WRITE_END]);

        // Get user input to write to pipe 1
        printf("Enter a message: ");

        scanf("%[^\n]%*c", write_msg);

        write(p1[WRITE_END], write_msg, strlen(write_msg) + 1);
        
        // Close the write end of pipe 1 and wait
        close(p1[WRITE_END]);

        wait(NULL);
        
        // Read from pipe 2 and print the message from the child process
        read(p2[READ_END], read_msg, BUFFER_SIZE);

        printf("Child process sent this message: %s\n", read_msg);
 
        // Close the read end of pipe 2
        close(p2[READ_END]);
    }
    // Child process (reads)
    else {
        // Close unused write end of the pipe 1 and read end of pipe 2
        close(p1[WRITE_END]);

        close(p2[READ_END]);
        
        // Read from pipe 1 and display the message
        read(p1[READ_END], write_msg, BUFFER_SIZE);

        printf("Child process received this message: %s\n", write_msg);

        // Close the read end of pipe 1
        close(p1[READ_END]);

        // Change the case of the message
        for (int i = 0; i < strlen(write_msg); i++)
        {
            if (isupper(write_msg[i]))
            {
                read_msg[i] = tolower(write_msg[i]);
            }
            else //this was also wrong,because it doesnt know 
                 //what to do if the character isnt upper or lower
                 //in our case it doesnt matter because spaces will just change to space.
            {
                read_msg[i] = toupper(write_msg[i]);
            }
        }

        // Write the message to pipe 2
        write(p2[WRITE_END], read_msg, strlen(read_msg) + 1); //this is what was wrong, you were taking the strlen of your write_msg still

        // Close the write end of pipe 2
        close(p2[WRITE_END]);
    }

    return 0;
}
