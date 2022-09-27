//
// Created by abhishek on 25/9/22.
//

#ifndef ISJ_RE_22_14571_HEADER_H
#define ISJ_RE_22_14571_HEADER_H
#include <string>
#include <vector>
#include "../util.h"
namespace http {
    const static std::string CRLF = "\r\n";
    const static std::string SPACE = " ";

    /* Http methods */
    enum class method {
        GET,
        POST,
        UNDEFINED
    };

    /* http version */
    enum class version {
        HTTP_1_0,
        HTTP_1_1,
        HTTP_2_0,
        UNDEFINED
    };

    /*
        utility functions
    */
    std::string to_string(method m);

    method to_method(std::string m);

    std::string to_string(version v);

    version to_version(std::string v);

    /*
        request header
    */
    struct header {
        std::string key;
        std::string value;

        header();
        header(std::string key, std::string value);
        std::string serialize();
        static header deserialize(std::string _header) {
            std::string key, value;
            std::vector<std::string> vec = tb_util::tokenize(_header, ":");
            key  = vec[0];
            value = tb_util::trim(vec[1]);

            return header(key, value);
        }

    };
}


#endif //ISJ_RE_22_14571_HEADER_H
