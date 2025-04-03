#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>    
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_ELEMENTS 1024

int main(int argc, char **argv) {
	int fd1[2];  // pipe 1
	int fd2[2];  // pipe 2
	pid_t pid_pfind;
	pid_t pid_sort;

	if (argc != 5 || strcmp(argv[1], "-d") != 0 || strcmp(argv[3], "-p") != 0) {
		fprintf(stderr, "Usage: %s -d <directory> -p <permissions string> [-h]\n", argv[0]);
		return EXIT_FAILURE;
	}


	if (pipe(fd1) < 0 || pipe(fd2) < 0){
		perror("Error: pipe failed");
		exit(EXIT_FAILURE);
	}
	// FOR PFIND
	if ((pid_pfind = fork())<0){
		perror("Error: pfind failed.");
		exit(EXIT_FAILURE);
		
	} else if (pid_pfind == 0) {  // child
		close(fd1[0]);
		dup2(fd1[1], STDOUT_FILENO);
		close(fd1[1]);
		
		//execlp("./pfind", "./pfind", NULL);
		//fprintf(stderr, "Error: pfind failed.\n");
		
		execlp("./pfind", "./pfind", "-d", argv[2], "-p", argv[4], NULL);
		perror("Error: pfind failed");
		exit(EXIT_FAILURE);
	
	}
	// FOR SORT
	if ((pid_sort = fork()) < 0){
    		perror("Error: fork failed");
		exit(EXIT_FAILURE);
	} else if (pid_sort == 0) {
    		close(fd1[1]); 
    		dup2(fd1[0], STDIN_FILENO); 
    		close(fd1[0]);

    		close(fd2[0]); 
    		dup2(fd2[1], STDOUT_FILENO);
    		close(fd2[1]);  // Close original descriptor

    		execlp("sort", "sort", NULL);
    		//fprintf(stderr, "Error: sort failed.\n");
		perror("Error: sort failed");
    		exit(EXIT_FAILURE);
	}
	// CLOSE PIPES
	close(fd1[0]); 
	close(fd1[1]);  
	close(fd2[1]);		
	
	char size[MAX_ELEMENTS];
    	int bytes, count = 0;
	// REMAINING PIPE
	while ((bytes = read(fd2[0], size, MAX_ELEMENTS - 1)) > 0) {
        	size[bytes] = '\0';
        	printf("%s", size);

        	// Count lines
        	for (int i = 0; i < bytes; i++) {
            		if (size[i] == '\n') {
                		count++;
            		}	
        	}
    	}	
    	close(fd2[0]); 

	waitpid(pid_pfind, NULL, 0);
	waitpid(pid_sort, NULL, 0);
	if (count > 0) { 
		printf("Total matches: %d\n", count);
	}	
	return EXIT_SUCCESS;
} 
