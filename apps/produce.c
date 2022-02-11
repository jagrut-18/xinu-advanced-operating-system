#include <xinu.h>
#include <prodcons.h>
#include <prodcons_bb.h>

void producer(int count, sid32 can_read, sid32 can_write) {
  // TODO: implement the following:
  // - Iterates from 0 to count (count including)
  //   - setting the value of the global variable 'n' each time
  //   - print produced value (new value of 'n'), e.g.: "produced : 8"
  for (int i = 0; i <= count; i++){
    wait(can_write);
    n = i;
    printf("produced : %d\n", n);
    signal(can_read);
  }
}

void producer_bb(int id, int count) {
  // TODO: implement the following:
  // - Iterate from 0 to count (count excluding)
  //   - add iteration value to the global array `arr_q`
  //   - print producer id (starting from 0) and written value as:
  //     "name : producer_X, write : X"
  for (int i = 0; i < count; i++){
    wait(can_write);
    arr_q[head] = i;
    head++;
    printf("name : producer_%d, write : %d\n", id, i);
    signal(can_read);
  }
  
}
