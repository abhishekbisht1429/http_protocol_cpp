//
// Created by abhishek on 25/9/22.
//

#include "http/http_exception.h"
namespace http {
    parse_exception::parse_exception(std::string msg): msg(msg) {}
    const char *parse_exception::what() {
        return msg.c_str();
    }

    http_exception::http_exception(std::string msg): msg(msg) {}
    const char *http_exception::what() {
        return msg.c_str();
    }
}