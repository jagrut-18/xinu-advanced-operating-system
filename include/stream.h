#include <xinu.h>
#include <future.h>

typedef struct data_element {
  int32 time;
  int32 value;
} de;

struct stream {
  sid32 spaces;
  sid32 items;
  sid32 mutex;
  int32 head;
  int32 tail;
  struct data_element** queue;
};

extern int stream_proc(int nargs, char *args[]);
extern int stream_proc_futures(int nargs, char *args[]);
