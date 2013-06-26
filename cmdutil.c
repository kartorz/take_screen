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

#include <stdlib.h>
#include <unistd.h>

#include "cmdutil.h"


int opt_frame_rate = 15;

char opt_ipaddr[7]= {'\0'};

unsigned opt_ipport = 8000;

char opt_omt[5] = "jpg";

int opt_function = FUN_SHOT;

void usage()
{
    printf("Usage: take_screen [OPTION] \n");
    printf("Take a screen shot, record screen, and projection\n \n");
    printf("-F  [=FUNCTION]     what you want to do. FUNCTION defaults to `shot', other valid value\n");
    printf("                  are `projection' and `recording'. More info below\n");
    printf("-r              set frame rate when recording screen or projection\n");
    printf("-i              set a ip address when projection\n");
    printf("-p              set a ip port when projection, default is 8000\n");
    printf("-o [=FMT]       specify picture format for shot, FMT defaults to `jpg' \n");
    printf("                  or can be `bmp' \n");
    printf("-O              specify the output path, default is `.'\n");
    printf("-h              display help and exit\n");
    printf("e.g.  \n");
    printf("take_screen -F shot -o [jpg|bmp] : take a screen shot \n");
    printf("take_screen -F projection -i 127.0.0.1 -p 8000 -r 15 :  like projection, we transfer screen to other host or device \n");
    printf("take_screen -F recording  -O [path]: Recording screen\n");
    exit(-1);
}

void parse_opt(int argc, char* argv[])
{
    int c;

    while((c = getopt(argc, argv, "ripoFh:")) != -1)
        switch (c)
        {
        case 'r':
        {
            opt_frame_rate = atoi(optarg);
            break;
        }
#ifdef HAS_NET
        case 'i':
        {
            strcpy(opt_ipaddr, optarg);
            break;
        }
        
        case 'p':
        {
            opt_ipport = atoi(optarg);
            break;
        }
#endif
        case 'o':
        {
            strcpy(opt_omt, optarg);
            break;
        }
        case 'F':
        {
            if(!strcmp(optarg,"shot"))
            {
                opt_function = FUN_SHOT;
            }else if(!strcmp(optarg, "projection")){
                opt_function = FUN_PROJECTION;
            }else if(!strcmp(optarg, "recording")){
                opt_function = FUN_RECORDING;
            }else
                usage();
            break;
        }
        case 'h':
            usage();
        default:
            usage();
        }
}


#if 0
void parse_opt(int argc, char* argv[])
{
    int i;

#define CHECK_NEXT_ARG(n) \
    if(argc-1-i < n ) \
        usage()

#define SET_REQUIRED_BITMAP() \
    opt_required_bitmap = opt_required_bitmap << 1; \
    opt_required_bitmap &= 0x01

    for(i=1; i<argc, i++)
    {
        if(!strcmp(argv[i], "-r") || !strcmp(argv[i], "--frame-rate") )
        {
            CHECK_NEXT_ARG(1);

            opt_frame_rate = atoi(argv[i+1]);
            ++i;
            continue;
        }
#ifdef HAS_NET
        if(!strcmp(argv[i], "-i") || !strcmp(argv[i], "--ip") )
        {
            CHECK_NEXT_ARG(1);

            strcpy(opt_ipaddr, argv[i+1]);
            ++i;
            continue;
        }
        
        if(!strcmp(argv[i], "-p") || !strcmp(argv[i], "--port") )
        {
            CHECK_NEXT_ARG(1);

            opt_ipport = atoi(argv[i+1]);
            ++i;
            continue;
        }
#endif
        if(!strcmp(argv[i], "-o"))
        {
            CHECK_NEXT_ARG(1);

            strcpy(opt_omt, argv[i+1]);
            ++i;
            continue;
        }
        
    }
}
#endif
