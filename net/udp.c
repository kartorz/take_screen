/*
 * This file comes from Internet, Thaks for the anoymous.
 * Copyright @ Anonymous
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>

#include "common.h"
#include "log.h"

typedef struct _HeadExt 
{ 
    //char   			flag[5]; // specify our data pocket.
    uint32_t   		m_nSeqNumber; // sequence of the frame.
    uint32_t       	m_nTotalFragment;// division of the frame.
    uint32_t 		m_nTotalSize;  // size of the fram
    // informat for every division.
    uint32_t 		m_nFragmentIndex; // index of a division of the frame.
    uint32_t 		m_usPayloadSize; // payload of a division.
    uint32_t 		m_usFragOffset;// offset to data of the whole frame.
    uint8_t 		m_bLastFragment;// "1" indicates last division.
 
 }__attribute__((packed)) HeadExt; 
 
 
#define RTP_SPLIT_PACKSIZE 1300
//The max payload for a udp packet is 1500. Generally we set to 1400，1300 is appropriate also. 
int count = 0;
//LONGLONG index = rand();


static int s_id = -1;
struct sockaddr_in addr;

void udp_send(const BYTE* pBuff, const uint32_t nLen) 
{ 
    HeadExt head; 
      
    head.m_nSeqNumber = ++count; 
 
    head.m_nTotalFragment = (nLen + RTP_SPLIT_PACKSIZE-1) /RTP_SPLIT_PACKSIZE; 
    head.m_nFragmentIndex = 0;
    head.m_nTotalSize = nLen; 
    head.m_bLastFragment = 1; 
    head.m_usFragOffset = 0; 

    LOG(LOG_INFO, "udp_send m_nTotalFragment:%d,  m_nTotalSize:%d\n",head.m_nTotalFragment, head.m_nTotalSize);

    BYTE tem[RTP_SPLIT_PACKSIZE+sizeof(HeadExt)]; 
 
    if(head.m_nTotalFragment == 1) 
    { 
        memcpy(tem,&head,sizeof(HeadExt)); 
        memcpy(tem+sizeof(HeadExt),pBuff,nLen); 
        head.m_usPayloadSize = nLen; 

 	   sendto(s_id,tem,nLen+sizeof(HeadExt),0,(struct sockaddr *)&addr, sizeof(addr));

       LOG(LOG_DEBUG,"udp_setd：m_nSeqNumber%d ,m_nTotalFragment %d,m_nFragmentIndex %d,m_usFragOffset %d,m_nTotalSize %d,m_nDataLen %d\r\n",\
           head.m_nSeqNumber,head.m_nTotalFragment,head.m_nFragmentIndex,head.m_usFragOffset,head.m_nTotalSize,head.m_usPayloadSize); 

        return; 
    } 
 
    int i = 0; 
    for( i = 0; i < head.m_nTotalFragment-1; i++)
    { 
        head.m_bLastFragment = 0; 
        head.m_nFragmentIndex = i; 
        head.m_usFragOffset = i*RTP_SPLIT_PACKSIZE; 
        head.m_usPayloadSize = RTP_SPLIT_PACKSIZE; 
        memcpy(tem,&head,sizeof(HeadExt)); 
        memcpy(tem+sizeof(HeadExt),pBuff+i*RTP_SPLIT_PACKSIZE,RTP_SPLIT_PACKSIZE); 
 		sendto(s_id,tem,sizeof(tem),0,(struct sockaddr *)&addr,sizeof(addr));

        LOG(LOG_DEBUG,"udp_send：m_nSeqNumber:(%d),m_nTotalFragment %d, \
m_nFragmentIndex %d,m_usFragOffset %d,m_nTotalSize %d,m_nDataLen %d\n",
            			head.m_nSeqNumber,
            			head.m_nTotalFragment,
						head.m_nFragmentIndex,
						head.m_usFragOffset,
						head.m_nTotalSize,
						head.m_usPayloadSize); 

    } 
 	int len = nLen - (i*RTP_SPLIT_PACKSIZE);
    head.m_bLastFragment = 1; 
    head.m_nFragmentIndex = i; 
    head.m_usFragOffset = i*RTP_SPLIT_PACKSIZE; 
    head.m_usPayloadSize = len; 
    memcpy(tem,&head,sizeof(HeadExt)); 
    memcpy(tem+sizeof(HeadExt),pBuff+i*RTP_SPLIT_PACKSIZE,len); 

 	sendto(s_id,tem,len+sizeof(HeadExt),0,(struct sockaddr *)&addr,sizeof(addr));

    LOG(LOG_DEBUG,"udp_send：m_nSeqNumber:(%d),m_nTotalFragment %d,m_nFragmentIndex %d,     \
m_usFragOffset %d, m_nTotalSize %d,m_nDataLen %d \n",
            			head.m_nSeqNumber,
            			head.m_nTotalFragment,
						head.m_nFragmentIndex,
						head.m_usFragOffset,
						head.m_nTotalSize,
						head.m_usPayloadSize); 
}

int udp_create_socket(char* ip, unsigned port)
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

void udp_close_socket()
{
    if(s_id != -1)
        close(s_id);
}
