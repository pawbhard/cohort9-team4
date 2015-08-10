#include<sys/types.h>
#include<netinet/in.h>
int
send_to_from(int fd, void *buf, size_t len, int flags,
        const struct sockaddr *to,
        const struct sockaddr *from,
        socklen_t tolen);

int
recv_from_to(int fd, void *buf, size_t len, int flags,
        struct sockaddr *from, 
        struct sockaddr *to,
        socklen_t sa_size);
