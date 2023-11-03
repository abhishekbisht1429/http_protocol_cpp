#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<iostream>
#include "net_socket/inet_server_socket.h"
#include "net_socket/socket_exception.h"

namespace net_socket {
    /* server net_socket */
    inet_server_socket::inet_server_socket() {
        server_sock_fd = socket(PF_INET, SOCK_STREAM, 0);
        if(server_sock_fd<0)
            throw socket_exception("failed to create net_socket");
    }

    void inet_server_socket::bind_address(ipv4_address addr, uint16_t port) {
        this->addr = sock_address(addr, port);
        struct sockaddr_in name;
        name.sin_family = AF_INET;
        if(inet_aton(addr.to_string().c_str(), &(name.sin_addr)) < 0)
            throw socket_exception("invalid ip address");
        name.sin_port = htons(port);

        if(bind(server_sock_fd, (struct sockaddr*)&name,
                sizeof(name)) < 0)
            throw socket_exception("failed to bind name");

        std::cout<<"successfully bound\n";
    }

    void inet_server_socket::bind_address(sock_address addr) {
        bind_address(addr.ip, addr.port);
    }

    void inet_server_socket::sock_listen(int n) {
        if(listen(server_sock_fd, n) < 0)
            throw socket_exception("unable to start listening");
    }

    inet_socket inet_server_socket::accept_connection() {
        sockaddr_in client_addr;
        socklen_t size = sizeof(client_addr);
        int new_sock = accept(server_sock_fd,
                              (struct sockaddr*)&client_addr,
                                      &size);
        if(new_sock<0)
            throw socket_exception("failed to accept connection");

        /* extract client info */
        ipv4_address client_ip =
                ipv4_address(inet_ntoa(client_addr.sin_addr));
        uint16_t client_port = ntohs(client_addr.sin_port);

        /* create an inet_socket object for communication */
        inet_socket socket_obj(new_sock, client_ip, client_port,
                               addr.ip, addr.port);

        return socket_obj;
    }

    void inet_server_socket::close_socket() {
        close(server_sock_fd);
    }
};