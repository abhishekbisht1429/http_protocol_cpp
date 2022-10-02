//
// Created by abhishek on 25/9/22.
//

#ifndef ISJ_RE_22_14571_SOCK_ADDRESS_H
#define ISJ_RE_22_14571_SOCK_ADDRESS_H

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

#endif //ISJ_RE_22_14571_SOCK_ADDRESS_H
