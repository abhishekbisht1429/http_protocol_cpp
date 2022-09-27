//
// Created by abhishek on 25/9/22.
//

#ifndef HTTP_PROTOCOL_CPP_IPV4_ADDRESS_H
#define HTTP_PROTOCOL_CPP_IPV4_ADDRESS_H
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

#endif //HTTP_PROTOCOL_CPP_IPV4_ADDRESS_H
