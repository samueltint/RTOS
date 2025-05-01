/*
By programing the handout 3 part 1, you will follow the lecture notes to implement the Named pipe (FIFO). In this handout, we have created 2 processes. the writing process -- L5-named_pipes_write.c and reading process L5-named_pipes_read.c. You need to compile the both c files and run them in two linux terminals. Once you open the two terminals, one is run the writing process which you can type the characters and you will see all the characters appear on the reading process terminal.  
This is a sample program of named pipe, used in conjunction with L5-named_pipes_write.c.
Execute the two programes in two separate terminals.
*/
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int fd1;

    // FIFO file path
    char * myfifo = "/tmp/myfifo";

    // Creating the named file(FIFO)
    // mkfifo(<pathname>,<permission>)
    mkfifo(myfifo, 0666);

    char str1[80], str2[80];
    while (1)
    {
        // First open in read only and read
        fd1 = open(myfifo,O_RDONLY);
        read(fd1, str1, 80);

        // Print the read string and close
        printf("From process 1: %s\n", str1);
        close(fd1);

        // Now open in write mode and write
        // string taken from user.
        fd1 = open(myfifo,O_WRONLY);
        fgets(str2, 80, stdin);
        write(fd1, str2, strlen(str2)+1);
        close(fd1);
    }
    return 0;
}