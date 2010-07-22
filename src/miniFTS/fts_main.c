#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <pthread.h>

#include "common.h"
#include "net_common.h"

#include "net_threader.h"
#include "net_messenger.h"
#include "unsigned_string.h"

#include "scar_log.h"
#include "miniFTS_commands.h"


int uricopy_pull (buffer_state_t * read_buffer_state, buffer_state_t * write_buffer_state);
int uricopy_push (buffer_state_t * read_buffer_state, buffer_state_t * write_buffer_state);

int uricopy_pull (buffer_state_t * read_buffer_state, buffer_state_t * write_buffer_state)
{
    unsigned char * p;

    if (strncmp(read_buffer_state -> buffer, URICOPY_PULL, strlen(URICOPY_PULL)) == 0)
    {
        p = &(read_buffer_state -> buffer[strlen(URICOPY_PULL) + 1]);
        scar_log (1, "%s: %s\n", __func__, p);

        if (vlet_get_srm (p, create_tmp_local_uri()))
            scar_log (1, "%s: Transfer failed\n", __func__);

        return 0;
    }
    else
        return -1;
}


int uricopy_push (buffer_state_t * read_buffer_state, buffer_state_t * write_buffer_state)
{
    unsigned char * p;

    if (strncmp(read_buffer_state -> buffer, URICOPY_PUSH, strlen(URICOPY_PUSH)) == 0)
    {
        p = &(read_buffer_state -> buffer[strlen(URICOPY_PUSH) + 1]);
        scar_log (1, "%s: %s\n", __func__, p);


        return 0;
    }
    else
        return -1;
}



int commander_active_io (buffer_state_t * read_buffer_state, buffer_state_t * write_buffer_state, void ** state)
{
    int rc                  = 0;

    fprintf (stderr, " << %s\n", read_buffer_state -> buffer);


    if (uricopy_pull (read_buffer_state, write_buffer_state) == 0)
        goto finalize_message_handling;

    if (uricopy_push (read_buffer_state, write_buffer_state) == 0)
        goto finalize_message_handling;


#if 0
    /* Fetch file to scratch file */
    fprintf (stderr, "perform: vlet_get_srm (\"srm://tbn18.nikhef.nl:8446/dpm/nikhef.nl/home/dteam/okoeroo/fuse/testfile\", \"file:///tmp/bar/foo\")\n");
    if (vlet_get_srm ("srm://tbn18.nikhef.nl:8446/dpm/nikhef.nl/home/dteam/okoeroo/fuse/testfile", "file:///tmp/bar/foo"))
    {
        /* File transfer to local scratch space failed */
        goto finalize_message_handling;
    }
    fprintf (stderr, "done.\n");
#endif

finalize_message_handling:
    /* *state = (void *) ftp_state; */
    return rc;
}


int commander_idle_io (buffer_state_t * write_buffer_state, void ** state)
{
    int rc = 0;

/* finalize_message_handling: */
    /* *state = (void *) ftp_state; */
    return rc;
}


int commander_state_initiator (void ** state, void * input_state)
{
    if (state)
        *state = NULL;

    return 0;
}

int commander_state_liberator (void ** state)
{
    return 0;
}



int main (int argc, char * argv[])
{
    scar_log_open (NULL, NULL, DO_ERRLOG);

    threadingDaemonStart (4000,
                          10,
                          4096, 
                          4096, 
                          commander_active_io, 
                          commander_idle_io,
                          commander_state_initiator,
                          NULL,
                          commander_state_liberator);
    return 0;
}
