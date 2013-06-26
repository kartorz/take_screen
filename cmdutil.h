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

#ifndef _CMDUTIL_H_
#define _CMDUTIL_H_

#define FUN_SHOT       1
#define FUN_PROJECTION 2
#define FUN_RECORDING  3

extern int opt_frame_rate;

extern char opt_ipaddr[7];

extern unsigned opt_ipport;

//jpeg, bmp, mp4.
extern char opt_omt[5];

extern int opt_function;

extern void parse_opt(int argc, char* argv[]);

extern void usage();

#endif
