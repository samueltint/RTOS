#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSIZE 512
#define err(mess) { fprintf(stderr,"Error: %s.", mess); exit(1); }

void main()
{
    int fd, n;
    char buf[BUFFSIZE];

    memset( buf, '\0', sizeof(char)*BUFFSIZE );

    if ( (fd = open("./data.txt", O_RDONLY)) < 0)
        err("open")

    /*read the data from FIIE ./data.txt*/
    while( (n = read(fd, buf, BUFFSIZE) ) > 0) {
	
	/* write the data to command line*/
        if ( write(STDOUT_FILENO, buf, n) != n) { 
            exit(1);
        }
    }
    close(fd);
}
