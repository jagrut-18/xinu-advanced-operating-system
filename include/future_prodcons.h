#include <future.h>
uint future_prod(future_t* fut, char* value);
uint future_cons(future_t* fut);
void future_prodcons(int nargs, char *args[]);

extern char *val;
extern sid32 print_sem;
