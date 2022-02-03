/* xsh_run.c - xsh_run */



#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include "shprototypes.h"

/*------------------------------------------------------------------------
 * xsh_run - A wrapper shell that runs internal functions
 *------------------------------------------------------------------------
 */

char functions[3][100] = { 
                            "hello",
                            "list",
                            "prodcons",
                        };

void printFunctions();

shellcmd xsh_run(int nargs, char *args[]) {


	/* Output info for '--help' argument */

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Usage: %s\n\n", args[0]);
		printf("Description:\n");
		printf("\tA wrapper shell that runs internal functions.\n");
		printf("\t--help\tdisplay this help and exit\n");
		return 0;
	}


	if ((nargs == 1) || (strncmp(args[1], "list", 4) == 0)) {
        printFunctions();
        return OK;
        }

        /* This will go past "run" and pass the function/process name and its arguments */
        args++;
        nargs--;

        if(strncmp(args[0], "hello", 5) == 0) {
            /* create a process with the function as an entry point. */
            resume (create((void *) xsh_hello, 4096, 20, "hello", 2, nargs, args));
        }
        else if(strncmp(args[0], "prodcons", 8) == 0) {
            /* create a process with the function as an entry point. */
            resume (create((void *) xsh_prodcons, 4096, 20, "prodcons", 2, nargs, args));
        }
        else {
            printFunctions();
            return OK;
        }

	return 0;
}

void printFunctions() {
    for (int i = 0; i < 3; i++){
        printf("%s\n", functions[i]);
    }
}
