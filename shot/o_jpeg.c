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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#include "common.h"
#include "outfmt.h"
#include "jpeglib.h"

char jpeg_fname[] = "/tmp/screen.jpg";
FILE* pf_jpeg;

static int init_jpeg(int argc, char *argv[])
{
    pf_jpeg = fopen(jpeg_fname, "wb");
    if(pf_jpeg == NULL)
        return -1;
    return 0;
}

static void net_send_jpeg()
{
    struct stat attr;
    BYTE *buf;

    if(stat(jpeg_fname, &attr) == -1)
        fatalerr("can't file jpeg's attribute\n");

    buf = malloc(attr.st_size);
    fread(buf, attr.st_size, 1, pf_jpeg);

    socket_send(buf, attr.st_size);

    free(buf);
}

static int write_jpeg(scimg* img)
{
    int w, h, stride;
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPROW buff[1];
    JSAMPLE * imgbuf = (JSAMPLE *)(img->data);
   
    if(img->data == NULL)
        return -1;

    w = img->w;
    h = img->h;

    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_compress(&cinfo);

    jpeg_stdio_dest(&cinfo, pf_jpeg);

	cinfo.image_width = w; 	
	cinfo.image_height = h;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;

	jpeg_set_defaults(&cinfo);

    //jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

    jpeg_start_compress(&cinfo, TRUE);
    
    stride = w * 3;

    LOG(LOG_DEBUG, "start to writing jpeg file, (%d,%d)\n", w, h);
    
     while (cinfo.next_scanline < cinfo.image_height) {
         buff[0] = & imgbuf[cinfo.next_scanline * stride];
         (void) jpeg_write_scanlines(&cinfo, buff, 1);
     }

   
    jpeg_finish_compress(&cinfo);
    
    net_send_jpeg();

    fclose(pf_jpeg);
    
    jpeg_destroy_compress(&cinfo);
}

static void uninit_jpeg()
{
    return ;
}

REGISTER_OUTPUT_MD(jpeg)
