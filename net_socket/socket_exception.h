//
// Created by abhishek on 25/9/22.
//

#ifndef ISJ_RE_22_14571_SOCKET_EXCEPTION_H
#define ISJ_RE_22_14571_SOCKET_EXCEPTION_H
#include <exception>
#include <string>
namespace net_socket {
    /* net_socket exception */
    class socket_exception : public std::exception {
        std::string msg;
        int err_code;
    public:
        socket_exception();
        socket_exception(std::string msg);
        socket_exception(std::string msg, int err_code);
        const char *what();
    };

    class socket_time_out_exception : public socket_exception {
    public:
        socket_time_out_exception();
    };

    class invalid_socket_descriptor_exception : public socket_exception {};

    class conn_refused_exception: public socket_exception {};

    class conn_reset_exception: public socket_exception {};

    class invalid_buf_addr_exception : public socket_exception {};

    class interrupt_exception : public socket_exception {};

    class no_connection_mode_execption : public socket_exception {};

    class invalid_args_exception : public socket_exception {};

    class mem_alloc_failed_exception : public socket_exception {};

    class sock_not_connected_exception : public socket_exception {};

    class not_sock_exception : public socket_exception {};

    class invalid_msgsize_exception : public socket_exception {};

    class is_connected_exception : public socket_exception {};

    class no_buf_available_exception : public socket_exception {};

    class flag_not_supported_exception : public socket_exception {};

    class permisison_exception : public socket_exception {};

    class address_in_use_exception : public socket_exception {};

    class address_not_available_exception : public socket_exception {};

    class address_format_unsupported_exception : public socket_exception {};

    class network_unreachable_exception : public socket_exception {};

    class protocol_unsupported_exception : public socket_exception {};

    class broken_pipe_exception : public socket_exception {};

}
#endif //ISJ_RE_22_14571_SOCKET_EXCEPTION_H
