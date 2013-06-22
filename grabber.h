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

#ifndef  _SCREEN_GRABBER_H_
#define  _SCREEN_GRABBER_H_

#include "type.h"
#include "scimg.h"

//return: On success, return 0, otherwise non-zero.
typedef int (*fn_init)(int argc, char* argv[]);
typedef void (*fn_uninit)(void);

//return: On success, return 0, otherwise non-zero.
typedef int (*fn_grab_screen)(scimg *);

 typedef struct {
     fn_init init_grabber;
     fn_uninit uninit_grabber;
     fn_grab_screen  grab_screen;
 }sc_grabber_t;

#define REGISTER_GRABBER(name) \
sc_grabber_t name ## _grabber = {  \
    .init_grabber = init_grabber, \
    .uninit_grabber = uninit_grabber, \
    .grab_screen = grab_screen_image, \
};


extern sc_grabber_t x11_grabber;

#endif
