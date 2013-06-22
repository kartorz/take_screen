/*
 * Copyright GPL
 */

#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

#include "log.h"

const char program_name[] = "TakeScreen";
const char log_path[] = "/tmp/takesceen.log";

static int output_tofile = 0;
static int output_level = LOG_DEBUG;
static int fd_log = -1;
static FILE * pflog = NULL;

//typedef void (*fatalerr_callback)(void);
void fatalerr(char *msg, ...)
{
	va_list args;
	fflush(stdout);
	fflush(stderr);
	fprintf(stderr, "%s: error: ", program_name);
	va_start(args, msg);
	vfprintf(stderr, msg, args);
	va_end(args);
	fprintf(stderr, "\n");
	exit(-1);
}

void LOG(int level,char *msg, ...)
{
	va_list args;

    if(level < output_level)
        return;

    if(!output_tofile)
    {
        va_start(args, msg);
        if(level == LOG_ERROR)
            vfprintf(stderr, msg, args);
        else
            vprintf(msg, args);
        va_end(args);
        printf("\n");
    }else
    {
        if(fd_log == -1)
        {
            fd_log = open(log_path, O_WRONLY|O_TRUNC);
            
            if(fd_log == -1)
            {
                printf("open %s failture, errno is (%d), don't write to file, using printf instead\n",log_path, errno);
                output_tofile = 0;
                return;
            }
            pflog = fdopen(fd_log, "r");
            if(!pflog)
            {
                output_tofile = 0;
                return;
            }
        }else {
            va_start(args, msg);
            vfprintf(pflog, msg, args);
            va_end(args);
        }
    }
}

