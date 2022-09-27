//
// Created by abhishek on 25/9/22.
//

#ifndef HTTP_PROTOCOL_CPP_SOCK_ADDRESS_H
#define HTTP_PROTOCOL_CPP_SOCK_ADDRESS_H

#include "ipv4_address.h"
namespace net_socket {

    struct sock_address {
        ipv4_address ip;
        uint16_t port;
        long long uid;

        sock_address();
        sock_address(ipv4_address ip, uint16_t port);
        sock_address(long long uid);
        long long get_uid();
    };

} // net_socket

#endif //HTTP_PROTOCOL_CPP_SOCK_ADDRESS_H
