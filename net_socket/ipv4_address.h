//
// Created by abhishek on 25/9/22.
//

#ifndef ISJ_RE_22_14571_IPV4_ADDRESS_H
#define ISJ_RE_22_14571_IPV4_ADDRESS_H
#include <string>
namespace net_socket {

    class ipv4_address {
        /*TODO: future functionality - add check for ip address correctness */
        std::string ip;
        public:
        ipv4_address();
        ipv4_address(std::string ip);
        std::string to_string();
    };

} // net_socket

#endif //ISJ_RE_22_14571_IPV4_ADDRESS_H
