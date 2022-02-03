#include <xinu.h>
#include <prodcons.h>

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
