// declare globally shared array
int arr_q[5];

// declare globally shared semaphores
sid32 can_read;
sid32 can_write;
sid32 end_process;

// declare globally shared read and write indices
int head;
int tail;

// function prototypes
void consumer_bb(int id, int count);
void producer_bb(int id, int count);


// global variable for exiting process
extern int values_remaining;
