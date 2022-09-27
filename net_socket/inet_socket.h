//
// Created by abhishek on 25/9/22.
//

#ifndef HTTP_PROTOCOL_CPP_INET_SOCKET_H
#define HTTP_PROTOCOL_CPP_INET_SOCKET_H

#include "sock_address.h"
namespace net_socket {

    class inet_socket {
        /* fd for net_socket */
        constexpr static int TIMEOUT = 10000000;
        int sock;
        sock_address remote_addr;
        sock_address local_addr;

        void throw_exception(int err);

    public:
        inet_socket();

        inet_socket(int sock, ipv4_address ip, uint16_t port,
                    ipv4_address local_ip, uint16_t local_port);

        void connect_server(std::string ip, uint16_t port);

        void connect_server(sock_address addr);

        int read_bytes(void *buf, int count);

        int write_bytes(void *buf, int count);

        sock_address get_remote_address();

        sock_address get_local_address();

        void close_socket();
    };

} // net_socket

#endif //HTTP_PROTOCOL_CPP_INET_SOCKET_H
