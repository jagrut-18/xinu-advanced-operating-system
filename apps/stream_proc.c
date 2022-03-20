#include <xinu.h>
#include <stream.h>
#include "tscdf_input.h"
#include "tscdf.h"
#include "exit_process.h"

uint port;
int num_streams, work_queue_depth, time_window, output_time;
void stream_consumer(int32 id, struct stream *str);
int stream_proc(int nargs, char *args[]);

int stream_proc(int nargs, char *args[])
{
    ulong secs, msecs, time;
    secs = clktime;
    msecs = clkticks;
    // TODO: Parse arguments
    char usage[] = "Usage: run tscdf -s <num_streams> -w <work_queue_depth> -t <time_window> -o <output_time>\n";

    int i;
    char *ch, c;
    if (nargs != 9)
    {
        printf("%s", usage);
        signal(exit_process);
        return SYSERR;
    }
    else
    {
        i = nargs - 1;
        while (i > 0)
        {
            ch = args[i - 1];
            c = *(++ch);

            switch (c)
            {
            case 's':
                num_streams = atoi(args[i]);
                break;

            case 'w':
                work_queue_depth = atoi(args[i]);
                break;

            case 't':
                time_window = atoi(args[i]);
                break;

            case 'o':
                output_time = atoi(args[i]);
                break;

            default:
                printf("%s", usage);
                signal(exit_process);
                return SYSERR;
            }

            i -= 2;
        }
    }

    // TODO: Create streams
    if ((port = ptcreate(num_streams)) == SYSERR)
    {
        printf("Error: Port Creation\n");
        signal(exit_process);
        return -1;
    }

    struct stream **streams = (struct stream **)getmem(num_streams * sizeof(struct stream *));

    // TODO: Create consumer processes and initialize streams
    // Use `i` as the stream id.
    for (int i = 0; i < num_streams; i++)
    {
        streams[i] = (struct stream *)getmem(sizeof(struct stream));
        streams[i]->head = 0;
        streams[i]->tail = 0;
        streams[i]->items = semcreate(0);
        streams[i]->mutex = semcreate(1);
        streams[i]->spaces = semcreate(work_queue_depth);
        streams[i]->queue = (de **)getmem(sizeof(de *) * work_queue_depth);

        for (int j = 0; j < work_queue_depth; j++)
        {
            streams[i]->queue[j] = (de *)getmem(sizeof(de));
        }

        resume(create((void *)stream_consumer, 2048, 20, "stream_consumer", 2, i, streams[i]));
    }

    // TODO: Parse input header file data and populate work queue
    for (int i = 0; i < number_inputs; i++)
    {
        char *a = (char *)stream_input[i];
        int st = atoi(a);
        while (*a++ != '\t')
            ;
        int ts = atoi(a);
        while (*a++ != '\t')
            ;
        int v = atoi(a);

        struct stream *stream = streams[st];
        de *prod;

        wait(stream->spaces);
        wait(stream->mutex);

        prod = stream->queue[stream->head];
        prod->time = ts;
        prod->value = v;

        stream->head++;
        stream->head %= work_queue_depth;

        signal(stream->mutex);
        signal(stream->items);
    }

    // TODO: Join all launched consumer processes
    for (i = 0; i < num_streams; i++)
    {
        uint32 pm = ptrecv(port);
        printf("process %d exited\n", pm);
    }

    ptdelete(port, 0);

    // TODO: Measure the time of this entire function and report it at the end

    time = (((clktime * 1000) + clkticks) - ((secs * 1000) + msecs));
    printf("time in ms: %u\n", time);
    signal(exit_process);
    return 0;
}


void stream_consumer(int32 id, struct stream *str)
{

    // TODO: Print the current id and pid
    kprintf("stream_consumer id:%d (pid:%d)\n", id, getpid());

    // TODO: Consume all values from the work queue of the corresponding stream

    struct tscdf *tc = tscdf_init(time_window);
    int count = 0;
    char *output;
    int *qarray = (int32 *)getmem(6 * sizeof(int32));

    while (1)
    {

        wait(str->items);
        wait(str->mutex);

        struct data_element *qElement = str->queue[str->tail];
        int update = tscdf_update(tc, qElement->time, qElement->value);
        if (qElement->time == 0)
        {
            str->tail++;
            str->tail %= work_queue_depth;
            signal(str->mutex);
            signal(str->spaces);
            break;
        }

        str->tail++;
        str->tail %= work_queue_depth;
        count++;

        if (update == SYSERR)
        {
            return;
        }

        if (count == output_time)
        {
            count = 0;
            output = "";
            qarray = tscdf_quartiles(tc);

            if (qarray == NULL)
            {
                kprintf("tscdf_quartiles returned NULL\n");
                continue;
            }

            sprintf(output, "s%d: %d %d %d %d %d", id, qarray[0], qarray[1], qarray[2], qarray[3], qarray[4]);
            kprintf("%s\n", output);
            freemem((char *)qarray, (6 * sizeof(int32)));
        }
        signal(str->mutex);
        signal(str->spaces);
    }

    tscdf_free(tc);
    kprintf("stream_consumer exiting\n");
    ptsend(port, getpid());
    return;
}