/* xsh_run.c - xsh_run */



#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "shprototypes.h"
#include "prodcons_bb.h"
#include "exit_process.h"
#include "future.h"
#include "future_prodcons.h"
/*------------------------------------------------------------------------
 * xsh_run - A wrapper shell that runs internal functions
 *------------------------------------------------------------------------
 */

char functions[5][100] = { 
                            "futest",
                            "hello",
                            "list",
                            "prodcons",
                            "prodcons_bb",
                        };

void printFunctions();
void prodcons_bb();
void a_test();
void b_test();
int isNumber(char n[]);
int future_fib(int nargs, char *args[]);
int future_free_test(int nargs, char *args[]);


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
        else if(strncmp(args[0], "futest", 6) == 0) {
            /* create a process with the function as an entry point. */
            // a_test();
            // b_test();
            future_prodcons(nargs, args);
            // wait(exit_process);
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
    for (int i = 0; i < 5; i++){
        printf("%s\n", functions[i]);
    }
}

int arr_q[5];
sid32 can_read_bb, can_write_bb;
int head, tail, values_remaining;

void prodcons_bb(int nargs, char *args[]) {
    if (nargs != 5) {
        fprintf(stderr, "Syntax: run prodcons_bb <# of producer processes> <# of consumer processes> <# of iterations the producer runs> <# of iterations the consumer runs>\n");
		signal(exit_process);
        return;
	}
  //create and initialize semaphores to necessary values
  can_read_bb = semcreate(0);
  can_write_bb = semcreate(1);

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
      return;
  }

  values_remaining = n_prod * i_prod;

  for (int i = 0; i < n_prod; i++){
      char name[10];
      sprintf(name, "producer_%d", i);
      resume(create(producer_bb, 1024, 20, name, 2, i, i_prod));
  }

  for (int i = 0; i < n_con; i++){
      char name[10];
      sprintf(name, "consumer_%d", i);
      resume(create(consumer_bb, 1024, 20, name, 2, i, i_con));
  }
}


void future_prodcons(int nargs, char *args[]) {

  // First, try to iterate through the arguments and make sure they are all valid based on the requirements
  // (you should not assume that the argument after "s" is always a number)
  if (nargs < 2){
      printf("Syntax: run futest [-pc [g ...] [s VALUE ...]|-f NUMBER][--free]\n");
    //   signal(exit_process);
      return;
  }
  if (strcmp(args[1], "-pc") != 0 && strcmp(args[1], "-f") != 0 && strcmp(args[1], "--free") != 0){
      printf("Syntax: run futest [-pc [g ...] [s VALUE ...]|-f NUMBER][--free]\n");
    //   signal(exit_process);
      return;
  }
  int i = 2;
  if (strcmp(args[1], "-pc") == 0){
    while (i < nargs) {
      // TODO: write your code here to check the validity of arguments
      // if (strcmp(args[i], "g") != 0 && strcmp(args[i], "s") != 0 && strcmp(args[i], "0") != 0 && atoi(args[i]) <= 0 ){
      if (strcmp(args[i], "g") != 0 && strcmp(args[i], "s") != 0 && isNumber(args[i]) != 0 ){
        printf("Syntax: run futest [-pc [g ...] [s VALUE ...]|-f NUMBER][--free]\n");
      //   signal(exit_process);
        return;
      }
      // if (strcmp(args[i], "s") == 0 && strcmp(args[i+1], "0") != 0 && atoi(args[i+1]) <= 0) {
      if (strcmp(args[i], "s") == 0 && isNumber(args[i+1]) != 0) {
        printf("Syntax: run futest [-pc [g ...] [s VALUE ...]|-f NUMBER][--free]\n");
      //   signal(exit_process);
        return;
      }
      // if ((atoi(args[i]) > 0 || strcmp(args[i], "0") == 0) && strcmp(args[i-1], "s") != 0){
      if (isNumber(args[i]) == 0 && strcmp(args[i-1], "s") != 0){
        printf("Syntax: run futest [-pc [g ...] [s VALUE ...]|-f NUMBER][--free]\n");
      //   signal(exit_process);
        return;
      }
      i++;
    }
  }
  else if(strcmp(args[1], "-f") == 0) {
    if (isNumber(args[2]) != 0) {
      printf("Syntax: run futest [-pc [g ...] [s VALUE ...]|-f NUMBER][--free]\n");
      //   signal(exit_process);
      return;
    }
    future_fib(nargs, args);
    return;
  }
  else {
    if (nargs != 2){
        printf("Syntax: run futest [-pc [g ...] [s VALUE ...]|-f NUMBER][--free]\n");
      //   signal(exit_process);
        return;
    }
    future_free_test(nargs, args);
    return;
  }

  print_sem = semcreate(1);
  future_t* f_exclusive;
  f_exclusive = future_alloc(FUTURE_EXCLUSIVE, sizeof(int), 1);
  char *val;

  int num_args = i;  // keeping number of args to create the array
  i = 2; // reseting the index
  val  =  (char *) getmem(num_args); // initializing the array to keep the "s" numbers

  // Iterate again through the arguments and create the following processes based on the passed argument ("g" or "s VALUE")
  while (i < nargs) {
    if (strcmp(args[i], "g") == 0) {
      char id[10];
      sprintf(id, "fcons%d",i);
      resume(create(future_cons, 2048, 20, id, 1, f_exclusive));
    }
    if (strcmp(args[i], "s") == 0) {
      i++;
      uint8 number = atoi(args[i]);
      val[i] = number;
      resume(create(future_prod, 2048, 20, "fprod1", 2, f_exclusive, &val[i]));
      sleepms(5);
    }
    i++;
  }
  sleepms(100);
  future_free(f_exclusive);
}

int isNumber(char n[]) {
    int i = 0;
    while(n[i] != '\0'){
        if (isdigit(n[i]) == 0){
            return -1;
        }
        i++;
    }
    return 0;
}

void a_test() 
{
    int const i = 0x10101010;
    int const j = 0x55555555;

    future_t *a = future_alloc(FUTURE_EXCLUSIVE, sizeof(i), 1);
    future_t *b = future_alloc(FUTURE_EXCLUSIVE, sizeof(j), 1);

    future_set(a, (char*)&i);
    future_set(b, (char*)&j);

    int ii;
    int jj;
        
    future_get(a, (char*)&ii);
    future_get(b, (char*)&jj);

    printf("0x%08X 0x%08X\n", i, j);
    printf("0x%08X 0x%08X\n", ii, jj);
    if (i != ii) panic("i != ii");
    if (j != jj) panic("i != jj");

    future_free(a);
    future_free(b);
}

void b_test() 
{
    unsigned int x[2] = {
        0xFF00FF00, 
        0x00FF00FF
    };
    unsigned int y[2];

    future_t *f = future_alloc(FUTURE_EXCLUSIVE, sizeof(x), 1);
    future_set(f, (char*)x);
    future_get(f, (char*)y);
    printf("0x%08X 0x%08X\n", x[0], x[1]);
    printf("0x%08X 0x%08X\n", y[0], y[1]);
    if (x[0] != y[0]) panic("index 0 doesn't match\n");
    if (x[1] != y[1]) panic("index 1 doesn't match\n");
    future_free(f);
}