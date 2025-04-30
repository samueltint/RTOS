/*
 *	----- 48450 -- week 5 lab ------ 
By programing the handout 3 part 1, you will follow the lecture notes to implement the Named pipe (FIFO). In this handout, we have created 2 processes. the writing process -- L5-named_pipes_write.c and reading process L5-named_pipes_read.c. You need to compile the both c files and run them in two linux terminals. Once you open the two terminals, one is run the writing process which you can type the characters and you will see all the characters appear on the reading process terminal.  
This is a sample program of named pipe, used in conjunction with L5-named_pipes_read.c.
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
    int fd;

    // FIFO file path
    char * myfifo = "/tmp/myfifo";

    // Creating the named file(FIFO)
    // mkfifo(<pathname>, <permission>)
    mkfifo(myfifo, 0666);

    char arr1[80], arr2[80];
    while (1)
    {
        // Open FIFO for write only
        fd = open(myfifo, O_WRONLY);

        // Take an input arr2ing from user.
        // 80 is maximum length
        fgets(arr2, 80, stdin);

        // Write the input arr2ing on FIFO
        // and close it
        write(fd, arr2, strlen(arr2)+1);
        close(fd);

        // Open FIFO for Read only
        fd = open(myfifo, O_RDONLY);

        // Read from FIFO
        read(fd, arr1, sizeof(arr1));

        // Print the read message
        printf("From process 2: %s\n", arr1);
        close(fd);
    }
    return 0;
}