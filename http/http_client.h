//
// Created by abhishek on 25/9/22.
//

#ifndef HTTP_PROTOCOL_CPP_HTTP_CLIENT_H
#define HTTP_PROTOCOL_CPP_HTTP_CLIENT_H
#include "../net_socket/inet_socket.h"
#include "../net_socket/sock_address.h"
#include "response.h"
#include "request.h"
namespace http {

    class http_client {
        net_socket::sock_address server_sockaddr;
        net_socket::inet_socket sock;

        response read_response();

        void write_request(request req);

        void reconnect();

    public:
        http_client();

        void connect(net_socket::sock_address server_addr);
        void connect(net_socket::ipv4_address ip, uint16_t port);

        void disconnect();

        response send_request(request req);

        template<typename Callback, typename... Args>
        static void f(http_client &client, request &req, Callback &callback, Args... args) {
            response res = client.send_request(req);
            callback(res, args...);
        }

        template<typename Callback, typename... Args>
        void send_request_async(request req, Callback callback, Args... args);
    };

} // http

#endif //HTTP_PROTOCOL_CPP_HTTP_CLIENT_H
