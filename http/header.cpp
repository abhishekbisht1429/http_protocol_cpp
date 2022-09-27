//
// Created by abhishek on 25/9/22.
//

#include "header.h"

namespace http {
    std::string to_string(method m) {
        switch(m) {
            case method::GET:
                return "GET";
            case method::POST:
                return "POST";
            default:
                return "";
        }
    }

    method to_method(std::string m) {
        if(m == "GET")
            return method::GET;
        else if(m == "POST")
            return method::POST;
        else
            return method::UNDEFINED;
    }

    std::string to_string(version v) {
        switch(v) {
            case version::HTTP_1_0:
                return "HTTP/1.0";
            case version::HTTP_1_1:
                return "HTTP/1.1";
            case version::HTTP_2_0:
                return "HTTP/2.0";
            default:
                return "";
        }
    }

    version to_version(std::string v) {
        if(v == "HTTP/1.0")
            return version::HTTP_1_0;
        else if(v == "HTTP/1.1")
            return version::HTTP_1_1;
        else if(v == "HTTP/2.0")
            return version::HTTP_2_0;
        else
            return version::UNDEFINED;
    }

    header::header() {}
    header::header(std::string key, std::string value): key(key), value(value) {}

    std::string header::serialize() {
        std::string str = "";
        str += this->key;
        str += ":"+SPACE;
        str += this->value;
        return str;
    }
}