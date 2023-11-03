//
// Created by abhishek on 25/9/22.
//

#ifndef ISJ_RE_22_14571_HTTP_EXCEPTION_H
#define ISJ_RE_22_14571_HTTP_EXCEPTION_H
#include <string>
#include <exception>
namespace http {
    /* parse exception */
    class parse_exception : public std::exception {
        std::string msg;

        public:
        parse_exception(std::string msg);
        const char *what();
    };

    class http_exception : public std::exception {
        std::string msg;
        public:
        http_exception(std::string msg);
        const char *what();
    };

    class content_length_missing_exception : public http_exception {
        public:
        content_length_missing_exception(): http_exception("missing Content-Length") {}
    };

    class no_such_header_exception : public http_exception {
        public:
        no_such_header_exception(): http_exception("no such header") {}
    };

    class remote_end_closed_exception : public http_exception {
        public:
        remote_end_closed_exception() : http_exception("remote end closed") {}
    };

    class invalid_http_request_exception : public http_exception {
    public:
        invalid_http_request_exception() : http_exception("Invalid HTTP Request") {}
    };
}
#endif //ISJ_RE_22_14571_HTTP_EXCEPTION_H
