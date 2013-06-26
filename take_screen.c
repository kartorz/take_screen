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
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "grabber.h"
#include "outfmt.h"
#include "net/udp.h"
#include "cmdutil.h"
#include "log.h"

sc_grabber_t* screen_grabber;
ofmt* o_frame;

char o_path[24] = "screen_shot";

static void check_usr_input()
{
    return;
}


static void net_send_jpeg()
{
    struct stat attr;
    BYTE *buf;
    FILE* f_jpeg;

    if(stat(o_path, &attr) == -1)
        fatalerr("can't file jpeg's attribute\n");

    buf = malloc(attr.st_size);

    f_jpeg = fopen(o_path, "rb");

    fread(buf, attr.st_size, 1, f_jpeg);

    LOG(LOG_DEBUG, "net_send_jpeg size %d\n", attr.st_size);
    udp_send(buf, attr.st_size);

    fclose(f_jpeg);

    free(buf);
}


int main(int argc, char* argv[])
{
    scimg img;

    parse_opt(argc, argv);

#ifdef HAS_PROJECTION
    if(opt_function == FUN_PROJECTION){
        if(opt_ipaddr[0] == '\0' )
        {
            LOG(LOG_DEBUG, "can't get ip addr, when using projection\n");
            usage();
        }else{
            LOG(LOG_DEBUG, "get ip address(%s, %d), enable projection\n", opt_ipaddr, opt_ipport);
            strcpy(o_path,"/tmp/screen.jpeg");
            udp_create_socket(opt_ipaddr, opt_ipport);
        }
    }
#endif

    screen_grabber = &x11_grabber;
    if(!strcmp(opt_omt, "jpg"))
    {
        o_frame = &o_jpeg;
        img.origin = ORI_UPP_LEFT;
        strcat(o_path, ".jpg");
    }else
    {
        o_frame = &o_bmp;
        img.origin = ORI_BOT_LEFT;
        strcat(o_path, ".bmp");
    }
    LOG(LOG_DEBUG, "output file(%s)\n", o_path);

    img.data = NULL;

    screen_grabber->init_grabber();
    o_frame->init(o_path);  
    
    if(opt_function != FUN_SHOT)
    {
        unsigned long dt;
        gettime();
        LOG(LOG_INFO, "projection, Now! \n");
        do{
            screen_grabber->grab_screen(&img);

            o_frame->write_frame(&img);

            net_send_jpeg();

            check_usr_input();
            
            dt = gettime() < 1000/opt_frame_rate;
            if(dt > 0)
            {
                LOG(LOG_DEBUG, "frame rate: %d, sleep: %lu\n", opt_frame_rate, dt);
                usleep(dt);
            }
        }while(1);
    }else
    {
        screen_grabber->grab_screen(&img);
        o_frame->write_frame(&img);
    }

    screen_grabber->uninit_grabber();
    o_frame->uninit();

    if(img.data != NULL)
        free(img.data);

    udp_close_socket();
    return 0;
}
