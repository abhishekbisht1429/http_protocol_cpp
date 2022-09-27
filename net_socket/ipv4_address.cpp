//
// Created by abhishek on 25/9/22.
//

#include "ipv4_address.h"

namespace net_socket {
    ipv4_address::ipv4_address(): ip("0.0.0.0") {}
    ipv4_address::ipv4_address(std::string ip): ip(ip) {}
    std::string ipv4_address::to_string() {
        return ip;
    }
} // net_socket