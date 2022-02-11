// declare globally shared array
extern int arr_q[5];

// declare globally shared semaphores
extern sid32 can_read_bb;
extern sid32 can_write_bb;

// declare globally shared read and write indices
extern int head;
extern int tail;

// function prototypes
void consumer_bb(int id, int count);
void producer_bb(int id, int count);


// global variable for exiting process
extern int values_remaining;
