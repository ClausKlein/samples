/*
   Copy the file named argv[1] to a new file named in argv[2].
*/
#include <err.h>    // err
#include <errno.h>  // errno
#include <fcntl.h>  // open
#include <stdio.h>  // printf
#include <stdlib.h> // exit
#include <string.h> // strcmp
#include <unistd.h> // read/write

#ifndef BUF_SIZE /* Allow "cc -D" to override definition */
#    define BUF_SIZE 1024
#endif

int main(int argc, char *argv[])
{
    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];

    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        printf("%s old-file new-file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /**
     * Open input and output files
     **/

    inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1) {
        err(errno, "opening file %s", argv[1]);
    }

    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP; /* rw--w---- */
    outputFd = open(argv[2], openFlags, filePerms);
    if (outputFd == -1) {
        err(errno, "opening file %s", argv[2]);
    }

    /**
     * Transfer data until we encounter end of input or an error
     **/

    while ((numRead = read(inputFd, buf, sizeof(buf))) > 0) {
        size_t len = (size_t)numRead; // XXX cast to prevent warning
        if (write(outputFd, buf, len) != numRead) {
            err(errno, "couldn't write whole buffer");
        }
    }

    if (numRead == -1) {
        err(errno, "read");
    }

    /**
     * close input and output files
     **/

    if (close(inputFd) == -1) {
        err(errno, "close input");
    }
    if (close(outputFd) == -1) {
        err(errno, "close output");
    }

    exit(EXIT_SUCCESS);
}
