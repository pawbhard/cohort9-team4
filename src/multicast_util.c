#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include"../include/multicast_util.h"
int
send_to_from(int fd, void *buf, size_t len, int flags,
        const struct sockaddr *to,
        const struct sockaddr *from,
        socklen_t tolen)
{
    struct iovec iov[1];
    struct msghdr msg;
    union {
        char cmsg[CMSG_SPACE(sizeof(struct in_pktinfo))];
    }u;
    struct cmsghdr* cmsgptr;
    
    //see man cmsg and sendmsg

    iov[0].iov_base = buf;
    iov[0].iov_len = len;

    memset(&u, 0, sizeof(u));

    memset(&msg, 0, sizeof(msg));
    msg.msg_name = (void *)(struct sockaddr *)to;
    msg.msg_namelen = tolen;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    msg.msg_control = &u;
    msg.msg_controllen = sizeof(u);
    msg.msg_flags = flags;

    cmsgptr = CMSG_FIRSTHDR(&msg);
    if (to->sa_family == AF_INET && from->sa_family == AF_INET) {
        struct in_pktinfo *pktptr;
        cmsgptr->cmsg_level = IPPROTO_IP;
        cmsgptr->cmsg_type = IP_PKTINFO;
        cmsgptr->cmsg_len = CMSG_LEN(sizeof(struct in_pktinfo));
        pktptr = (struct in_pktinfo *)(CMSG_DATA(cmsgptr));
        pktptr->ipi_spec_dst = ((struct sockaddr_in*)from)->sin_addr;
    }
    msg.msg_controllen = cmsgptr->cmsg_len;

    return sendmsg(fd, &msg, flags);
}

int
recv_from_to(int fd, void *buf, size_t len, int flags,
        struct sockaddr *from, struct sockaddr *to,
        socklen_t sa_size)
{
    struct iovec iov[1];
    union {
        char cmsg[CMSG_SPACE(sizeof(struct in_pktinfo))];
    } u;
    struct cmsghdr *cmsgptr;
    struct msghdr msg;
    socklen_t recv_length;

    iov[0].iov_base = buf;
    iov[0].iov_len = len;

    memset(&msg, 0, sizeof(msg));
    msg.msg_name = (struct sockaddr *)from;
    msg.msg_namelen = sa_size;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    msg.msg_control = &u;
    msg.msg_controllen = sizeof(u);

    recv_length = recvmsg(fd, &msg, flags);
    if (recv_length < 0)
        return recv_length;

    /* Here we try to retrieve destination IP and memorize it */
    for (cmsgptr = CMSG_FIRSTHDR(&msg);
            cmsgptr != NULL;
            cmsgptr = CMSG_NXTHDR(&msg, cmsgptr)
    ) {
        if (cmsgptr->cmsg_level == IPPROTO_IP
         && cmsgptr->cmsg_type == IP_PKTINFO
        ) {
#define pktinfo(cmsgptr) ( (struct in_pktinfo*)(CMSG_DATA(cmsgptr)) )
            to->sa_family = AF_INET;
            ((struct sockaddr_in*)to)->sin_addr = pktinfo(cmsgptr)->ipi_addr;
#undef pktinfo
            break;
        }
    }
    return recv_length;
}
