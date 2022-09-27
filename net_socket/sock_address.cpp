//
// Created by abhishek on 25/9/22.
//

#include "sock_address.h"
#include "../util.h"

namespace net_socket {
    sock_address::sock_address(): ip("0.0.0.0"), port(0){
        uid = 0;
    }
    sock_address::sock_address(ipv4_address ip, uint16_t port): ip(ip), port
            (port) {
        auto parts = tb_util::tokenize(ip.to_string(), ".");
        long long p1 = std::stoll(parts[0]);
        long long p2 = std::stoll(parts[1]);
        long long p3 = std::stoll(parts[2]);
        long long p4 = std::stoll(parts[3]);
        long long p5 = port;
        uid = p1 | (p2<<8) | (p3<<16) | (p4<<24) | (p5<<32);
    }

    sock_address::sock_address(long long uid) {
        long long mask = 0x00000000000000ffl;
        long long p1 = uid & mask;
        mask <<= 8;
        long long p2 = (uid & mask)>>8;
        mask <<=8;
        long long p3 = (uid & mask)>>16;
        mask <<=8;
        long long p4 = (uid & mask)>>24;

        mask = (0x000000000000ffffl)<<32;
        long long p5 = (uid & mask)>>32;

        port = (uint16_t)p5;
        ip = std::to_string(p1)+"."+std::to_string(p2)+
             "."+std::to_string(p3)+"."+std::to_string(p4);
        this->uid = uid;
    }

    long long sock_address::get_uid() {
        return uid;
    }
} // net_socket