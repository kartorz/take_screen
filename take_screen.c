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

#include "grabber.h"
#include "outfmt.h"
#include <stdlib.h>

sc_grabber_t* screen_grabber;
ofmt* output_frame;

int main(int argc, char* argv[])
{
    scimg img;
    int delay = 5;
    screen_grabber = &x11_grabber;
    if(1)
    {
        output_frame = &o_jpeg;
        img.origin = ORI_UPP_LEFT;
    }else
    {
        output_frame = &o_bmp;
        img.origin = ORI_BOT_LEFT;
    }

    screen_grabber->init_grabber(argc, argv);
    output_frame->init(argc, argv);

    create_socket("127.0.0.1", 9900);
    
    sleep(delay);

    screen_grabber->grab_screen(&img);

    output_frame->write_frame(&img);

    screen_grabber->uninit_grabber();
    output_frame->uninit();

    if(img.data != NULL)
        free(img.data);

    return 0;
}
