#ifndef _UDP_H_
#define _UDP_H_

extern void udp_send(const BYTE* pBuff, const uint32_t nLen);

extern int udp_create_socket(char* ip, unsigned port);

extern void udp_close_socket(void);

#endif
