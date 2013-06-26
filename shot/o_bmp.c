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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "grabber.h"
#include "outfmt.h"
#include "log.h"

#pragma pack(push, 2)
typedef struct {
  uint8_t  id[2];           // offset
  uint32_t filesize;        // 2
  uint32_t reserved;        // 6
  uint32_t headersize;      // 10
  uint32_t infoSize;        // 14
  uint32_t width;           // 18
  uint32_t height;          // 22
  uint16_t biPlanes;       // 26
  uint16_t bits;           // 28
  uint32_t biCompression;   // 30
  uint32_t biSizeImage;     // 34
  uint32_t biXPelsPerMeter; // 38
  uint32_t biYPelsPerMeter; // 42
  uint32_t biClrUsed;       // 46
  uint32_t biClrImportant;  // 50
} BMPHEAD;
#pragma pack(pop)

static FILE* pf_bmp = NULL;

static int init_bmp(char *path)
{
    pf_bmp = fopen(path, "w");
    if(pf_bmp == NULL)
        return -1;
    return 0;
}

static int write_bmp(scimg* img)
{
    // create a 24bit BMP header
    BMPHEAD bh;
    BYTE* pixels = img->data;

    if(img->depth != 24)
    {
        LOG(LOG_ERROR, "the bits of bmp is %d\n", img->depth);
        return -1;
    }

    LOG(LOG_DEBUG, "write screen to bmp file\n");

    memset((char *)&bh,0,sizeof(BMPHEAD));

    memcpy(bh.id,"BM",2);
    bh.headersize = 54L;
    bh.infoSize = 0x28L;
    bh.width = img->w;
    bh.height = img->h;
    bh.biPlanes = 1;
    bh.bits = 24;
    bh.biCompression = 0L;

    //number of bytes per line in a BMP is divisible by 4
    long bytesPerLine = bh.width * 3;
    if (bytesPerLine & 0x0003)
    {
        bytesPerLine |= 0x0003;
        ++bytesPerLine;
    }
    // filesize = headersize + bytesPerLine * number of lines
    bh.filesize = bh.headersize + bytesPerLine * bh.height;
        
    fwrite(&bh.id, sizeof(bh),1, pf_bmp);

    BYTE *lineBuf = malloc(bytesPerLine);
    if(!lineBuf)
        return -1;

    memset(lineBuf, 0, bytesPerLine);
    // lines are stored in BMPs upside down
    UINT y = 1;
    for (; y <= (UINT) bh.height ; ++y) //compensate for gl's inverted Y axis
    {
        BYTE *s = (BYTE *)pixels + (y - 1) * 3 * bh.width;
        BYTE *d = lineBuf;
        UINT x = 0;
        for (; x < (UINT) bh.width; x++)
        {
            *d++ = *(s + x * 3 + 2);
        	*d++ = *(s + x * 3 + 1);
            *d++ = *(s + x * 3);
            //format for storing is BGR
        }

        fwrite(lineBuf, bytesPerLine, 1, pf_bmp);
    }

    free(lineBuf);
      
    fclose(pf_bmp);

    return 0;
}

static void uninit_bmp(void)
{
    return ;
}


REGISTER_OUTPUT_MD(bmp)
