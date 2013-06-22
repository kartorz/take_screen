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

#ifndef  _OUTPUT_H_
#define  _OUTPUT_H__

#include "scimg.h"
#include "grabber.h"

typedef int (*fn_write)(scimg *);

 typedef struct {
     fn_init init;
     fn_uninit uninit;
     fn_write write_frame;
 }ofmt;

#define  REGISTER_OUTPUT_MD(module_name) \
    ofmt o_ ## module_name = { \
        .init = init_ ## module_name ,           \
        .uninit = uninit_ ## module_name ,       \
        .write_frame = write_ ## module_name ,  \
};

extern ofmt o_bmp;
extern ofmt o_jpeg;
#endif
