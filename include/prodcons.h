/* Global variable for producer consumer */
extern int n; /* this is just declaration */

/* Function Prototype */
void consumer(int count, sid32 can_read, sid32 can_write);
void producer(int count, sid32 can_read, sid32 can_write);

/* Semaphores */
sid32 can_read;
sid32 can_write;