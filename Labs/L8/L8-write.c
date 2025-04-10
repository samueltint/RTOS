#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFSIZE 512
#define err(mess) { fprintf(stderr,"Error: %s.", mess); exit(1); }

void main()
{
    int fd, n;

    char buf[BUFFSIZE];

    memset( buf, '\0', sizeof(char)*BUFFSIZE );

    mkfifo("fifo_x", 0666);
    if ( (fd = open("./data-1.txt", O_WRONLY)) < 0)
        err("open")
    
    /* read a line from command line*/
    while( (n = read(STDIN_FILENO, buf, BUFFSIZE) ) > 0) {

	/*write the buffer to FILE ./data-1.txt*/
        if ( write(fd, buf, strlen(buf)) != strlen(buf)) { 
            err("write");
        }
	
	/*clear the buffer*/
	memset( buf, '\0', sizeof(char)*BUFFSIZE );
    //exit(1);
    }
    close(fd);
    
}
