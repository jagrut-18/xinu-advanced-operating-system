#include <xinu.h>
#include <prodcons.h>

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
}
