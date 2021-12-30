#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
 // IPv4  header len without options  ICMP echo req   and ping google
#define IPv4HL 20
#define ICMPHL 8 
#define SOURCE_IP "10.0.0.5"
#define Dest "8.8.4.4"
// checksum
unsigned short checksumM(unsigned short * paddress, int len);
int main (){
  // IPv4 , ICMP headers
  struct ip iphdr;
  struct icmp icmphdr;
  char Data_[IP_MAXPACKET] = "##~~~~~~~~~~~~~~~~~~ pinging .. pinging .. pinging ~~~~~~~~~~~~~~~~~~##\n";
  int datalen = strlen(Data_)+1;
  struct  timeval start,end;
  double t1,t2;	
  // echo request ICMP header code 0 and ID to copy response
  icmphdr.icmp_seq = 0;
  icmphdr.icmp_type = ICMP_ECHO;
  icmphdr.icmp_code = 0;
  icmphdr.icmp_cksum = 0;
  icmphdr.icmp_id = 18; // hai
  char Packet_[IP_MAXPACKET];
  //adding ICMP header and data
  memcpy (Packet_ , &icmphdr, ICMPHL);
  memcpy (Packet_+ICMPHL, Data_, datalen);
  icmphdr.icmp_cksum = checksumM((unsigned short *) Packet_ , ICMPHL + datalen);
  memcpy ((Packet_), &icmphdr, ICMPHL);
  struct sockaddr_in dest_in;
  memset (&dest_in, 0, sizeof (struct sockaddr_in));
  dest_in.sin_addr.s_addr = inet_addr(Dest);
  dest_in.sin_family = AF_INET;
//socket creation
  int Socket_ = -1;
  if ((Socket_ = socket (AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1){
    fprintf (stderr, "Error %d with socket", errno);
    fprintf (stderr, "Should run by Admin user\n");
    return -1;
    }
    t2=0.0; 
    t1=0.0;
    gettimeofday(&start,NULL);
    if (sendto (Socket_,Packet_,ICMPHL+datalen+IPv4HL,0,(struct sockaddr*)&dest_in,sizeof(dest_in))==-1){
        fprintf (stderr, "Error %d srnd too", errno);
	  }	
    int loop=1; 
    while (loop){
    int l=sizeof(dest_in);
    if (recvfrom(Socket_, Packet_, sizeof(Packet_),0,(struct sockaddr*)&dest_in,&l )>0){ 
	  gettimeofday(&end,NULL);	
	  printf("rtt [%lu] ms\n",((end.tv_sec-start.tv_sec)*1000000 +end.tv_usec -start.tv_usec)/1000);
    break;    
    }
    close(Socket_);
    return 0;
    }
  }
// checksum
  unsigned short checksumM(unsigned short * paddress, int len){
  int Total = 0;
	int nleft = len;
	unsigned short * w = paddress;
	unsigned short answer = 0;
	while (nleft > 1){
	Total += *w++;
	nleft -= 2;
	}
	if (nleft == 1){
		*((unsigned char *)&answer) = *((unsigned char *)w);
		Total += answer;
	}
	Total = (Total >> 16) + (Total & 0xffff);
	Total += (Total >> 16);
	answer = ~Total;
	return answer;
}