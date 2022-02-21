#include <future.h>
#include <xinu.h>

future_t* future_alloc(future_mode_t mode, uint size, uint nelem) {
  intmask mask;
  mask = disable();
  future_t* new_future;
 // TODO: write your code here
  if (mode == FUTURE_EXCLUSIVE) {
      new_future = (future_t*) getmem(sizeof(future_t));
      new_future->state = FUTURE_EMPTY; 
      new_future->data = (char*) getmem(size);
      new_future->size = size;
      new_future->mode = mode;
  }
  restore(mask);
  return new_future;
}

// TODO: write your code here for future_free, future_get and future_set

syscall future_free(future_t* f) {
    intmask mask;
    mask = disable();
    kill(f->pid);
    restore(mask);
    syscall output = freemem((char*)f, sizeof(future_t));
    return output;
}

// ----------------------------------------------------------------------

syscall future_get(future_t* f,  char* out) {
    intmask mask;
    mask = disable();
    if (f->mode == FUTURE_EXCLUSIVE) {
        if (f->state == FUTURE_WAITING) {
            restore(mask);
            return SYSERR;
        }
        if (f->state == FUTURE_EMPTY) {
            f->pid = getpid();
            f->state = FUTURE_WAITING;
            suspend(getpid());
            *out = *f->data;
            restore(mask);
            return OK;
        }
        if (f->state == FUTURE_READY) {
            *out = *f->data;
            f->state = FUTURE_EMPTY;
            restore(mask);
            return OK;
        }
    }

}

// ----------------------------------------------------------------------

syscall future_set(future_t* f, char* in) {
    intmask mask;
    mask = disable();
    if (f->mode == FUTURE_EXCLUSIVE) {
        if (f->state == FUTURE_EMPTY) {
            *f->data = *in;
            f->state = FUTURE_READY;
            restore(mask);
            return OK;
        }
        if (f->state == FUTURE_WAITING) {
            *f->data = *in;
            f->state = FUTURE_EMPTY;
            resume(f->pid);
            restore(mask);
            return OK;
        }
        if (f->state == FUTURE_READY) {
            restore(mask);
            return SYSERR;
        }
    }
}
