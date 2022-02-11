#include <xinu.h>
#include <prodcons.h>
#include <prodcons_bb.h>
#include <exit_process.h>

void consumer(int count, sid32 can_read, sid32 can_write) {
  // TODO: implement the following:
  // - Iterates from 0 to count (count including)
  //   - reading the value of the global variable 'n' each time
  //   - print consumed value (the value of 'n'), e.g. "consumed : 8"
  for (int i = 0; i <= count; i++){
    wait(can_read);
    printf("consumed : %d\n", n);
    signal(can_write);
  }
  signal(exit_process);
}

void consumer_bb(int id, int count) {
  // TODO: implement the following:
  // - Iterate from 0 to count (count excluding)
  //   - read the next available value from the global array `arr_q`
  //   - print consumer id (starting from 0) and read value as:
  //     "name : consumer_X, read : X"
  int val;
  for (int i = 0; i < count; i++){
    wait(can_read_bb);
    val = arr_q[tail];
    printf("name : consumer_%d, read : %d\n", id, val);
    tail  = (tail + 1) % 5;
    values_remaining--;
    signal(can_write_bb);
    if (values_remaining == 0){
      signal(exit_process);
    }
  }
}

