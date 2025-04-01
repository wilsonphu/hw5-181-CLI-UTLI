#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>     
#include <sys/stat.h>  
#include <unistd.h>  

bool str_check (const char *perm) {
    for (int i = 0; i < 9; i++) {
        switch (i%3) {
            case 0:
		if (perm[i] != 'r' && perm[i] != '-') {
                    return false;
                }
		break;
            case 1:
		if (perm[i] != 'w' && perm[i] != '-') {
                    return false;
                }
		break;
            case 2:
		if (perm[i] != 'x' && perm[i] != '-') {
                    return false;
                }
                break;
	    default: 
		    return false;
	}
    }
    return true;   
}

void recursive_search(const char *dir, const char *perm_string){
	DIR *dirp;
	struct dirent *entry;
	struct stat statbuf;
	char path[PATH_MAX];

	if ((dirp = opendir(dir)) == NULL){
			fprintf(stderr, "Error: Cannot open directory '%s'. %s.\n", dir, strerror(errno));
			return;
	}

	while ((entry = readdir(dirp)) != NULL){
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) { 
			continue;
		}

		//build full path
		snprintf(path, sizeof(path), "%s/%s", dir, entry->d_name);

		//retrive file info
		if (lstat(path, &statbuf) < 0) {
           		fprintf(stderr, "Error: Cannot stat '%s'. %s.\n", path, strerror(errno));
			continue;
		}
		if (S_ISDIR(statbuf.st_mode)) {
        		recursive_search(path, perm_string);
        	}	

        	// if it's a regular file, check permissions
        	else if (S_ISREG(statbuf.st_mode)) {
            		char actual_perm[10];

            	        //create the permission string based on the file's mode
            	        actual_perm[0] = '-';
            		actual_perm[1] = (statbuf.st_mode & S_IRUSR) ? 'r' : '-';
            		actual_perm[2] = (statbuf.st_mode & S_IWUSR) ? 'w' : '-';
            		actual_perm[3] = (statbuf.st_mode & S_IXUSR) ? 'x' : '-';
            		actual_perm[4] = (statbuf.st_mode & S_IRGRP) ? 'r' : '-';
            		actual_perm[5] = (statbuf.st_mode & S_IWGRP) ? 'w' : '-';
            		actual_perm[6] = (statbuf.st_mode & S_IXGRP) ? 'x' : '-';
            		actual_perm[7] = (statbuf.st_mode & S_IROTH) ? 'r' : '-';
            		actual_perm[8] = (statbuf.st_mode & S_IWOTH) ? 'w' : '-';
            		actual_perm[9] = '\0'; 

                	if (strcmp(actual_perm, perm_string) == 0) {
                		printf("%s\n", path); 
            		}	
        	}	
    	}	  
    	closedir(dirp);
}

int main(int argc, char **argv) {

    int dflag = 0, pflag = 0, c;
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
    
    if (dflag = 0 || pflag = 0) { 
        if (dflag = 1 && pflag = 0) {
	    fprintf(stderr, "Error: Required argument -p <permissions string> not found.\n");
	    return EXIT_FAILURE;
        if (dflag = 0 && pflag = 1) {
  	    fprintf(stderr, "Error: Required argument -d <directory> not found.\n");
	    return EXIT_FAILURE;
        } 
    }

    if (perm == NULL || strlen(perm) != 9) { return EXIT_FAILURE; }
     
    if (str_check(perm) == false) { 
        fprintf(stderr, "Error: Permissions string '%s' is invalid.", perm);
 	return EXIT_FAILURE;
    } 

    recursive_search(dir,perm);
    // will check if return true or false
    // if false print to stderr and return EXIT FAILURE
    // else continue     
 

    return EXIT_SUCCESS;

}
