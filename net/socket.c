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

#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "log.h"

static int s_id = -1;
struct sockaddr_in addr;

int create_socket(char* ip, unsigned port)
{
	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(port);
	
	if ((s_id = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
        LOG(LOG_DEBUG,"can't open socket\n");
		return -1;
	}
    LOG(LOG_DEBUG, "create socket succesfully \n");
    return s_id;
}

void close_socket()
{
    if(s_id != -1)
        close(s_id);
}

extern udp_send(const int s_id, const struct sockaddr_in* addr, const char* pBuff, const uint32_t nLen);

void socket_send(char* pBuff, uint32_t nLen)
{
    udp_send(s_id, &addr, pBuff, nLen);
}
