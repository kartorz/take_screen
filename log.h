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

#ifndef _LOG_H_
#define _LOG_H_

typedef enum{
    LOG_NONE,
    LOG_DEBUG,
    LOG_ERROR,
    LOG_INFO,
}log_level;

extern void LOG(int level,char *msg, ...);
extern void fatalerr(char *msg, ...);


#endif
