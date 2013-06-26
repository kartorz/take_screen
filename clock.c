/* 
 *   Copyright (c) 2013
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *    If you want to get more infomation about,see <http://www.gnu.org/licenses/>.
 * 
 */

#include <stdio.h>
#include <sys/time.h>

static unsigned long  base_time = 0;

unsigned long gettime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    if(base_time == 0)
        base_time = tv.tv_sec*1000+tv.tv_usec;

    return (tv.tv_sec*1000+tv.tv_usec - base_time);
}

unsigned long getabstime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec*1000+tv.tv_usec);
}

unsigned long getbasetime()
{
    return base_time;
}



