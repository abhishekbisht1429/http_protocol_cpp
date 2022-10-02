//
// Created by abhishek on 25/9/22.
//

#ifndef ISJ_RE_22_14571_INET_SERVER_SOCKET_H
#define ISJ_RE_22_14571_INET_SERVER_SOCKET_H
#include "inet_socket.h"
namespace net_socket {
    class inet_server_socket {
        /* net_socket descriptor */
        int server_sock_fd;
        /* local address */
        sock_address addr;

    public:
        inet_server_socket();

        void bind_address(ipv4_address addr, uint16_t port);

        void bind_address(sock_address addr);

        void sock_listen(int n);

        inet_socket accept_connection();

        void close_socket();
    };
}
#endif //ISJ_RE_22_14571_INET_SERVER_SOCKET_H
