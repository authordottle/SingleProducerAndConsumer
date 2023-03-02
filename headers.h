/***** headers.h ******/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define BUFFER_SIZE 10

typedef struct {
    int id;                                // item id
    char data[100];                        // item data
} item;

typedef struct {
	int in;                                // shared variable in
	int out;                               // shared variable out
	item buffer[BUFFER_SIZE];              // bounded buffer with size BUFFER_SIZE
} shared_struct;

