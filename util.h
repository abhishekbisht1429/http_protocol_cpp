//
// Created by abhishek on 25/9/22.
//


#ifndef ISJ_RE_22_14571_UTIL_H
#define ISJ_RE_22_14571_UTIL_H
//#include <functional>
//#include <map>
//#include <string_view>
//#include <sys/socket.h>
//#include <poll.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <unistd.h>
//#include <exception>
//#include <string>
#include <vector>
//#include <iostream>
//#include <thread>
//#include <regex>
//#include <cstring>

namespace tb_util {
    //TODO: remove the usuage of bytes and replace with string
//    typedef std::basic_string<uint8_t> bytes;
//    typedef std::basic_istringstream<uint8_t> bytes_istream;
//    typedef std::basic_ostringstream<uint8_t> bytes_ostream;


    std::string ltrim(const std::string &s);

    std::string rtrim(const std::string &s);

    std::string trim(const std::string &s);

    std::vector<std::string> tokenize(std::string str, std::string delim);

    std::string join(std::string path1, std::string path2);

    std::string expand_tilde(std::string path);

    std::vector<std::string> tokenize_command(std::string inp);

//    bytes s2b(std::string str);
//
//    std::string b2s(bytes bs);

    std::string serialize_string_vec(std::vector<std::string> vec);

    std::vector<std::string> deserialize_string_vec(std::string b);

//    void output_bytes(std::ostream &os, bytes &b);
};

#endif //ISJ_RE_22_14571_UTIL_H
