#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
int main(){
  int RetV,Socket_fd,i,n;
  socklen_t clilen;
  struct sockaddr clientddr, serveraddr;
  char buf[10000]; 
  Socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  if (Socket_fd < 0){perror("Socket");exit(1);}
  clilen = sizeof(struct sockaddr);    
  printf("Before receiving\n");   
  while(1){
  n=recvfrom(Socket_fd,buf,10000,0,(struct sockaddr *)&clientddr,&clilen);
  struct iphdr *ip_hdr = (struct iphdr *)buf;
  struct icmphdr *icmp_hdr = (struct icmphdr *)((char *)ip_hdr + (4 * ip_hdr->ihl));
  char source [16];
  char dest [16];
  printf("Destination ip [%s] Source ip [%s] code [%d] type [%d]\n",inet_ntop(AF_INET,&ip_hdr->daddr,dest,sizeof(source)),inet_ntop(AF_INET,&ip_hdr->saddr,source,sizeof(source)),icmp_hdr->code,icmp_hdr->type);
  }
}