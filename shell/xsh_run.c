/* xsh_run.c - xsh_run */



#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "shprototypes.h"
#include "prodcons_bb.h"
#include "exit_process.h"
/*------------------------------------------------------------------------
 * xsh_run - A wrapper shell that runs internal functions
 *------------------------------------------------------------------------
 */

char functions[4][100] = { 
                            "hello",
                            "list",
                            "prodcons",
                            "prodcons_bb",
                        };

void printFunctions();

shellcmd xsh_run(int nargs, char *args[]) {

    //Semaphore to wait until execution is done
    exit_process = semcreate(0);
    
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
            wait(exit_process);
        }
        else if(strncmp(args[0], "prodcons_bb", 11) == 0) {
            /* create a process with the function as an entry point. */
            prodcons_bb(nargs, args);
            wait(exit_process);
        }
        else if(strncmp(args[0], "prodcons", 8) == 0) {
            /* create a process with the function as an entry point. */
            resume (create((void *) xsh_prodcons, 4096, 20, "prodcons", 2, nargs, args));
            wait(exit_process);
        }
        else {
            printFunctions();
            return OK;
        }

	return 0;
}

void printFunctions() {
    for (int i = 0; i < 4; i++){
        printf("%s\n", functions[i]);
    }
}

int values_remaining;

void prodcons_bb(int nargs, char *args[]) {
    if (nargs != 5) {
        fprintf(stderr, "Syntax: run prodcons_bb <# of producer processes> <# of consumer processes> <# of iterations the producer runs> <# of iterations the consumer runs>\n");
		return 1;
	}
  //create and initialize semaphores to necessary values
  can_read = semcreate(0);
  can_write = semcreate(1);

  //initialize read and write indices for the queue
  head = 0;
  tail = 0;

  //create producer and consumer processes and put them in ready queue
  int n_prod = atoi(args[1]);
  int n_con = atoi(args[2]);
  int i_prod = atoi(args[3]);
  int i_con = atoi(args[4]);
  
  if ((n_prod * i_prod) != (n_con * i_con)){
      fprintf(stderr, "Iteration Mismatch Error: the number of producer(s) iteration does not match the consumer(s) iteration\n");
      signal(exit_process);
      return OK;
  }

  values_remaining = n_prod * i_prod;

  for (int i = 0; i < n_prod; i++){
      resume(create(producer_bb, 1024, 20, "producer_bb", 2, i, i_prod));
  }

  for (int i = 0; i < n_con; i++){
      resume(create(consumer_bb, 1024, 20, "consumer_bb", 2, i, i_con));
  }

}
