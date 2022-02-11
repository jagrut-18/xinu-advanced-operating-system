/* xsh_prodcons.c - xsh_prodcons */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <prodcons.h>

/*------------------------------------------------------------------------
 * xsh_prodcons - producer and consumer
 *------------------------------------------------------------------------
 */

int n;

shellcmd xsh_prodcons(int nargs, char *args[]) {


	/* Output info for '--help' argument */

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Usage: %s\n\n", args[0]);
		printf("Description:\n");
		printf("\tProducer and Consumer\n");
		printf("\t--help\tdisplay this help and exit\n");
		return 0;
	}

	/* Check argument count */

	if (nargs > 2) {
		fprintf(stderr, "Syntax: run prodcons [counter]\n");
		return 1;
	}
	// if (nargs > 2) {
	// 	fprintf(stderr, "%s: too many arguments\n", args[0]);
	// 	fprintf(stderr, "Try '%s --help' for more information\n",
	// 		args[0]);
	// 	return 1;
	// }
	
    int count = 200;
	can_read = semcreate(0);
	can_write = semcreate(1);

    if (nargs == 2){
        char *new_count = args[1];
        count = atoi(new_count);
    }

    resume(create(consumer, 1024, 20, "consumer", 3, count, can_read, can_write));
    resume(create(producer, 1024, 20, "producer", 3, count, can_read, can_write));
	return 0;
}
