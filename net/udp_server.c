
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>

#define RTP_SPLIT_PACKSIZE 1500
typedef struct _HeadExt 
{ 
   //char   			flag[5];
    uint32_t   		m_nSeqNumber;
    uint32_t       	m_nTotalFragment;
    uint32_t 		m_nTotalSize; 

    uint32_t 		m_nFragmentIndex; 
    uint32_t 		m_usPayloadSize;
    uint32_t 		m_usFragOffset;
    uint8_t 		m_bLastFragment;
 
 }__attribute__((packed)) HeadExt;
 
 typedef struct _Packet
 {
 	bool isUse;
 	bool isFull;
 	uint8_t recvedpacks; 
    uint16_t recvedbytes; 
    uint32_t seqnum; 
 	char *data;
 
 }__attribute__((packed)) Packet;
 
int sin_len;
int s_id;
struct sockaddr_in sin;
static Packet packet[10];
bool Revc(char *tembuf);
void printfError(int error);

static uint32_t lastpacketseqnum = 0; 

int main()
{	
	int i = 0;
	for (; i < 10; i++)
	{
		packet[i].isUse = false;
	}
	char tembuf[RTP_SPLIT_PACKSIZE];
	bzero(&sin,sizeof(sin));
 	sin.sin_family = AF_INET;
 	sin.sin_addr.s_addr = htonl(INADDR_ANY);
 	sin.sin_port = htons(8000);
 	sin_len = sizeof(sin);

 	if ((s_id = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printfError(-1);
		return -1;
	}
	
 	if (bind(s_id,(struct sockaddr *)&sin,sizeof(sin)) < 0)
	{
	 	printfError(-2);
	 	return -2;
	}
	
	while(1)
	{
		Revc(tembuf);
		
		usleep(200*1000);
	}
	
	close(s_id);
	return 0;
}

bool Revc(char *tembuf)
{

	int ret = recvfrom(s_id,tembuf,RTP_SPLIT_PACKSIZE,0,(struct sockaddr *)&sin,&sin_len);
	if(ret <= 0)
	{
		return false;
	}
	//
	HeadExt *pheadext = (HeadExt *)tembuf;
	printf("dddd222:%d---%d\n",pheadext->m_usPayloadSize,pheadext->m_nSeqNumber);
	if (pheadext->m_usPayloadSize <= 0 || lastpacketseqnum > pheadext->m_nSeqNumber)
	{
		return false;
	}

	if (lastpacketseqnum+10 < pheadext->m_nSeqNumber)
	{

		int i = 0;
		for(;i < 10; i++)
		{
			if (packet[i].isUse && packet[i].seqnum == lastpacketseqnum + 1)
			{
				//
				packet[i].isUse = false;
				packet[i].recvedpacks = 0;
				packet[i].recvedbytes = 0;
				packet[i].seqnum = 0;
				free(packet[i].data);
				break;
			}
		}
		lastpacketseqnum++;
	}

	int i = 0;
	Packet *tem = NULL;
	bool isFull = false;
	bool isNewPacket = true;
	int fullindex = -1;
	//
	//
	for(; i < 10; i++)
	{
		if (packet[i].isUse && packet[i].seqnum == pheadext->m_nSeqNumber)
		{
			packet[i].recvedpacks++;
			packet[i].recvedbytes += pheadext->m_usPayloadSize;
			memcpy(packet[i].data+pheadext->m_usFragOffset,tembuf+sizeof(HeadExt),
			pheadext->m_usPayloadSize);
			isNewPacket = false;
			if (packet[i].recvedpacks == pheadext->m_nTotalFragment)
			{
				isFull = true;
				fullindex = i;
				packet[i].isFull = true;
			}
			break;
		}
	}
	//
	//
	if (isNewPacket)
	{	
		int index = -1;
		for(i = 0; i < 10; i++)
		{
			if (!packet[i].isUse)
			{
				index = i;
				break;
			}
		
		}
		//

		packet[index].isUse = true;
		packet[index].recvedpacks = 1;
		packet[index].recvedbytes = pheadext->m_usPayloadSize;
		packet[index].seqnum = pheadext->m_nSeqNumber;
		packet[index].data = (char *)malloc(pheadext->m_nTotalSize);
		memcpy(packet[index].data+pheadext->m_usFragOffset,tembuf+sizeof(HeadExt),
			pheadext->m_usPayloadSize);
		if (packet[index].recvedpacks == pheadext->m_nTotalFragment)
		{
			isFull = true;
			fullindex = index;
			packet[i].isFull = true;
		}
	}
		
	//
	if (fullindex > -1) 	
	{	
	
		if (packet[fullindex].seqnum == lastpacketseqnum+1)
		{	
	
			printf("packet seqnum %d \n",packet[fullindex].seqnum);
			packet[fullindex].isUse = false;
			packet[fullindex].recvedpacks = 0;
			packet[fullindex].recvedbytes = 0;
			packet[fullindex].seqnum = 0;
			free(packet[fullindex].data);
			lastpacketseqnum++;
		}
        else if (lastpacketseqnum + 5 < packet[fullindex].seqnum)
		{	int i = 0;
			for(;i < 10; i++)
			{
				if (packet[i].isUse && packet[i].seqnum == lastpacketseqnum + 1)
				{
				
					packet[i].isUse = false;
					packet[i].recvedpacks = 0;
					packet[i].recvedbytes = 0;
					packet[i].seqnum = 0;
					free(packet[i].data);
					break;
				}
			}
			lastpacketseqnum++;
			
			int j = 0;
			bool flag = false;
			bool ishas = false;
			for(i = 0; i < 10; i++)
			{	
				flag = false;
				ishas = false;
				for(j = 0; j < 10; j++)
				{
					if(packet[j].isUse && packet[j].seqnum == lastpacketseqnum+1)
					{
						ishas = true;
						if(packet[j].isFull)
						{
						
							printf("packet seqnum %d \n",packet[j].seqnum);
							packet[j].isUse = false;
							packet[j].recvedpacks = 0;
							packet[j].recvedbytes = 0;
							packet[j].seqnum = 0;
							free(packet[j].data);
							flag = true;
							lastpacketseqnum++;
							
						}
						break;
					}
									
				}
			
				if(!ishas)
				{
					flag = true;
					lastpacketseqnum++;
				}
				
			
				if (!flag)
				{
					break;
				}
			
			}
		}		
	}
	
}


void printfError(int error)
{	
	
	switch(error)
	{
		case -1:
			printf("socket fail\n");
			break;
		case -2:
			printf(" bind socket fail\n");
			break;
		default :
			printf("unkown error\n");
		break; 
	}
}
