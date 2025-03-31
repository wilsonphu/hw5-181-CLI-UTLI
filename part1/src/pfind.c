#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <string.h>
#include <stdbool.h>

static bool str_check (const char *perm) {
    for (int i = 0; i < 9; i++) {
        switch (perm[i]) {
            case 'r':
            case 'w':
            case 'x':
            case '-':
  	        break;
	    default:
		return false;
	}
    }
    return true;   
}


int main(int argc, char **argv) {

    int dflag = 0, pflag = 0, hflag = 0, c;
    opterr = 0;

    char *dir = NULL;
    char *perm = NULL;

    while ((c = getopt(argc, argv, "d:p:h")) != -1) {
        switch (c) {
	    case 'd':
 		dflag = 0;
		dir = optarg;
		break;
	    case 'p':
		pflag = 0;
		perm = optarg;
		break;
	    case 'h':
		fprintf(stdout,
			"Usage: ./pfind -d <directory> -p <permissions string> [-h]\n");
		return EXIT_SUCCESS;
	    case '?':
		fprintf(stderr, "Error: Invalid option '-%c' received.\n", optopt);	

	    default:
		return EXIT_FAILURE;
        }
    }

    if (strlen(perm) != 9) { return EXIT_FAILURE } 
    // will check if return true or false
    // if false print to stderr and return EXIT FAILURE
    // else continue     

   


    return EXIT_SUCCESS;
}
