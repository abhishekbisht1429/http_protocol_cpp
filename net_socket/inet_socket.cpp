//
// Created by abhishek on 25/9/22.
//
#include<functional>
#include<map>
#include<sys/socket.h>
#include<poll.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<vector>
#include<iostream>
#include<thread>
#include<regex>

#include "net_socket/socket_exception.h"
#include "net_socket/inet_socket.h"

namespace net_socket {
    void inet_socket::throw_exception(int err) {
        if(err & EBADF) {
            throw invalid_socket_descriptor_exception();
        } else if(err & ECONNREFUSED) {
            throw conn_refused_exception();
        } else if(err & EFAULT) {
            throw invalid_buf_addr_exception();
        } else if(err & EINTR) {
            throw interrupt_exception();
        } else if(err & EINVAL) {
            throw invalid_args_exception();
        } else if(err & ENOMEM) {
            throw mem_alloc_failed_exception();
        } else if(err & ENOTCONN) {
            throw sock_not_connected_exception();
        } else if(err & ENOTSOCK) {
            throw not_sock_exception();
        } else if(err & ECONNRESET) {
            throw conn_reset_exception();
        } else if(err & EDESTADDRREQ) {
            throw no_connection_mode_execption();
        } else if(err & EMSGSIZE) {
            throw invalid_msgsize_exception();
        } else if(err & ENOBUFS) {
            throw no_buf_available_exception();
        } else if(err & EOPNOTSUPP) {
            throw flag_not_supported_exception();
        } else if(err & EISCONN) {
            throw is_connected_exception();
        } else if(err & (EACCES | EPERM)) {
            throw permisison_exception();
        } else if(err & EADDRINUSE) {
            throw address_in_use_exception();
        } else if(err & EADDRNOTAVAIL) {
            throw address_not_available_exception();
        } else if(err & EAFNOSUPPORT) {
            throw address_format_unsupported_exception();
        } else if(err & ENETUNREACH) {
            throw network_unreachable_exception();
        } else if(err & EPROTOTYPE) {
            throw protocol_unsupported_exception();
        } else if(err & EPIPE) {
            throw broken_pipe_exception();
        }
    }

    inet_socket::inet_socket() {
        /* create net_socket */
        sock = socket(PF_INET, SOCK_STREAM, 0);
        if(sock<0)
            throw socket_exception("failed to create net_socket");
    }

    inet_socket::inet_socket(int sock, ipv4_address ip, uint16_t port,
                ipv4_address local_ip, uint16_t local_port) {
        this->sock = sock;
        remote_addr = sock_address(ip, port);
        local_addr = sock_address(local_ip, local_port);
    }

    void inet_socket::connect_server(std::string ip, uint16_t port) {
        remote_addr = sock_address(ipv4_address(ip), port);

        /* create server name C lib obj */
        struct sockaddr_in server_name;
        server_name.sin_family = AF_INET;
        if(inet_aton(ip.c_str(), &(server_name.sin_addr)) < 0)
            throw socket_exception("failed to connect");
        server_name.sin_port = htons(port);

        /* Connect to the server  */
        if(connect(sock, (struct sockaddr*)&server_name,
                   sizeof(server_name)) < 0)
            throw_exception(errno);

        /* obtain local address of net_socket */
        struct sockaddr_in local_name;
        socklen_t local_name_size = sizeof(local_name);
        getsockname(sock, (struct sockaddr*)&local_name, &
                (local_name_size));

        ipv4_address local_ip = ipv4_address(inet_ntoa(local_name.sin_addr));
        uint16_t local_port = ntohs(local_name.sin_port);

        local_addr = sock_address(local_ip, local_port);
    }

    void inet_socket::connect_server(sock_address addr) {
        connect_server(addr.ip.to_string(), addr.port);
    }

    int inet_socket::read_bytes(void *buf, int count) {
//        std::cout<<std::this_thread::get_id()<<" net_socket: reading bytes "
//                                               "from: "<<sock<<"\n";
//        std::cout<<std::this_thread::get_id()<<" has net_socket "<<sock<<"\n";
        pollfd pfd;
        pfd.fd = sock;
        pfd.events = POLLIN;
        int rc = poll(&pfd, 1, TIMEOUT);

        /* TODO: handle all the revents flags */
        if(rc > 0 && pfd.revents & POLLIN) {
            int actual_count;
            if((actual_count = recv(sock, buf, count, 0)) < 0) {
                throw_exception(errno);
            }
            return actual_count;
        } else if(rc == 0) {
            throw socket_time_out_exception();
        } else {
            throw socket_exception("error occured during polling");
        }
    }

    int inet_socket::write_bytes(void *buf, int count) {
//        std::cout<<"net_socket: writing bytes\n";
        int actual_count;
        try {
            if((actual_count = send(sock, buf, count,
                                    MSG_NOSIGNAL)) < 0) {
                throw_exception(errno);
            }
        } catch(std::exception &e) {
            std::cout<<e.what()<<"\n";
        }
        return actual_count;
    }

    sock_address inet_socket::get_remote_address() {
        return remote_addr;
    }

    sock_address inet_socket::get_local_address() {
        return local_addr;
    }

    void inet_socket::close_socket() {
        std::cout<<"Thread "<<std::this_thread::get_id()<<": closing net_socket "<<sock<<"\n";
        close(sock);
    }
} // net_socket