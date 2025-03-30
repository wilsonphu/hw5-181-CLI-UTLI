#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <string.h>


int main(int argc, char **argv) {

    int dflag = 0, pflag = 0, hflag = 0, c;
    opterr = 0;
    while ((c = getopt(argc, argv, "dph")) != -1) {
        switch (c) {
	    case 'd':
 		dflag = 0;
		break;
	    case 'p':
		pflag = 0;
		break;
	    case 'h':
		fprintf(stdout,
			"Usage: ./pfind -d <directory> -p <permissions string> [-h]\n");
		return EXIT_SUCCESS;
	    case '?':
		fprintf(stderr, "Error: Invalid option '-%c'\n", optopt);	


	    default:
		return EXIT_FAILURE;

        }

    }



}
