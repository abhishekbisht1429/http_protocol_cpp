//
// Created by abhishek on 25/9/22.
//
#include "socket_exception.h"
namespace net_socket{
    socket_exception::socket_exception() = default;
    socket_exception::socket_exception(std::string msg): msg(msg) {}
    socket_exception::socket_exception(std::string msg, int err_code): msg(msg), err_code(err_code) {}
    const char *socket_exception::what() {
        return msg.c_str();
    }

    socket_time_out_exception::socket_time_out_exception(): socket_exception("Socket timed out") {}
}