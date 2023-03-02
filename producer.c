/******* producer.c *******/
#include "headers.h"

int main()
{
	const char *name = "OS";
	int shm_fd;
	shared_struct *ptr;
	pid_t  pid;

	/* fork another process */
	pid = fork();
	if (pid < 0) { /* error occurred */
		printf("Fork Failed\n");
		exit(-1);
	}
	else if (pid == 0) { /* child process */
		sleep(1);
		execlp("./consumer", "consumer", NULL);
	}
	else { /* parent process */
		
		int size;
		int id = 0;
		int end = 0;
		FILE * pFile = fopen ("input.txt","r");
		if (pFile == NULL) {
			printf("fopen failed\n");
			exit(-1);
		}

		/* create the shared memory segment */
    		shm_fd = shm_open(name, O_CREAT|O_RDWR, 0666);
    		if (shm_fd == -1) {
        		printf("Shared memory failed\n");
        		exit(-1);
    		}

    		/* configure the size of the shared memory segment */
    		size = sizeof(shared_struct);
    		ftruncate(shm_fd, size);

    		/* now map the shared memory segment in the address space of the process */
    		ptr = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);
    		if (ptr == MAP_FAILED) {
        		printf("Map failed\n");
        		exit(-1);
    		}

    		/* initialize in and out */
    		ptr->in = 0;
    		ptr->out = 0;

    		/* write data */
    		while (1) {
			item elem;
			elem.id = id++;
			/* Read each line from the input file */
			if (fgets(elem.data, 100, pFile) == NULL) {
				elem.id = -1;
				elem.data[0] = 0;
				end = 1;
			}
			
        		/* buffer is full */
        		while (((ptr->in + 1) % BUFFER_SIZE) == ptr->out);
        		
			/* put an item in buffer */
        		ptr->buffer[ptr->in] = elem;
        		ptr->in = (ptr->in + 1) % BUFFER_SIZE;

			/* if there is no more data to be read from the file */
			if (end) break;
    		}
	}
}
