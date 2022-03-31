#include <future.h>
#include <xinu.h>

future_t* future_alloc(future_mode_t mode, uint size, uint nelem) {
  intmask mask;
  mask = disable();
  future_t* new_future;
 // TODO: write your code here
    new_future = (future_t *) getmem(sizeof(future_t));
    new_future->data = getmem(size * nelem);
    new_future->state = FUTURE_EMPTY;
    new_future->size = size;
    new_future->mode = mode;
    if (mode == FUTURE_SHARED) {
        new_future->get_queue = newqueue();
    }
    if (mode == FUTURE_QUEUE) {
        new_future->get_queue = newqueue();
        new_future->set_queue = newqueue();
        new_future->max_elems = nelem;
        new_future->count = 0;
        new_future->head = 0;
        new_future->tail = 0;
    }
  restore(mask);
  return new_future;
}

// TODO: write your code here for future_free, future_get and future_set

syscall future_free(future_t* f) {
    intmask mask;
    mask = disable();
    syscall memFreeOutput = freemem(f->data, f->size);
    if (memFreeOutput == SYSERR) {
        return SYSERR;
    }
    if (f->mode == FUTURE_EXCLUSIVE){
        kill(f->pid);
    }
    if (f->mode == FUTURE_SHARED) {
        while(nonempty(f->get_queue)){
            kill(dequeue(f->get_queue));
        }
    }
    if (f->mode == FUTURE_QUEUE) {
        while(nonempty(f->get_queue)){
            kill(dequeue(f->get_queue));
        }
        while(nonempty(f->set_queue)){
            kill(dequeue(f->set_queue));
        }
    }
    restore(mask);
    syscall output = freemem((char*)f, sizeof(future_t));
    return output;
}

// ----------------------------------------------------------------------

syscall future_get(future_t* f,  char* out) {
    intmask mask;
    mask = disable();

    if (f->mode == FUTURE_QUEUE) {
        if (f->count == 0){
            enqueue(getpid(), f->get_queue);
            suspend(getpid());
        }

        char* headelemptr = f->data + (f->head * f->size);
        memcpy(out, headelemptr, f->size);
        f->head = (f->head + 1) % f->max_elems;
        f->count--;

        if(nonempty(f->set_queue)) {
            resume(dequeue(f->set_queue));
        }

        restore(mask);
        return OK;
    }

    if (f->mode == FUTURE_EXCLUSIVE) {
        if (f->state == FUTURE_WAITING) {
            restore(mask);
            return SYSERR;
        }
        if (f->state == FUTURE_EMPTY) {
            f->pid = getpid();
            f->state = FUTURE_WAITING;
            suspend(getpid());
            memcpy(out, f->data, f->size);
            restore(mask);
            return OK;
        }
        if (f->state == FUTURE_READY) {
            memcpy(out, f->data, f->size);
            f->state = FUTURE_EMPTY;
            restore(mask);
            return OK;
        }
    }

    if (f->mode == FUTURE_SHARED) {
        intmask mask;
        mask = disable();
        if (f->state == FUTURE_EMPTY){
            enqueue(getpid(), f->get_queue);
            f->state = FUTURE_WAITING;
            suspend(getpid());
            memcpy(out, f->data, f->size);
            restore(mask);
            return OK;
        }
        if (f->state == FUTURE_WAITING) {
            enqueue(getpid(), f->get_queue);
            suspend(getpid());
            memcpy(out, f->data, f->size);
            restore(mask);
            return OK;
        }
        if (f->state == FUTURE_READY) {
            memcpy(out, f->data, f->size);
            restore(mask);
            return OK;
        }

        restore(mask);
        return SYSERR;
    }

}

// ----------------------------------------------------------------------

syscall future_set(future_t* f, char* in) {
    intmask mask;
    mask = disable();
        if (f->mode == FUTURE_QUEUE) {
            if (f->count == f->max_elems){
                enqueue(getpid(), f->set_queue);
                suspend(getpid());
            }

            char* tailelemptr = f->data + (f->tail * f->size);
            memcpy(tailelemptr, in, f->size);
            f->tail = (f->tail + 1) % f->max_elems;
            f->count++;

            if (nonempty(f->get_queue)){
                resume(dequeue(f->get_queue));
            }

            restore(mask);
            return OK;
        }
        if (f->state == FUTURE_EMPTY) {
            memcpy(f->data, in, f->size);
            f->state = FUTURE_READY;
            restore(mask);
            return OK;
        }
        if (f->state == FUTURE_READY) {
            restore(mask);
            return SYSERR;
        }
    if (f->mode == FUTURE_EXCLUSIVE) {
        if (f->state == FUTURE_WAITING) {
            memcpy(f->data, in, f->size);
            f->state = FUTURE_EMPTY;
            resume(f->pid);
            restore(mask);
            return OK;
        }
    }
    if (f->mode == FUTURE_SHARED) {
        if (f->state == FUTURE_WAITING){
            memcpy(f->data, in, f->size);
            f->state = FUTURE_EMPTY;
            while(nonempty(f->get_queue)){
                resume(dequeue(f->get_queue));
            }
            restore(mask);
            return OK;
        }
    }
    
}
