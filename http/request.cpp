//
// Created by abhishek on 25/9/22.
//

#include "http/request.h"

namespace http {
    request::request(method _method, std::string resource, version _version,
                     std::map<std::string, header> &headers, std::string body):
    _method(_method), resource(resource), headers(headers), _version(_version),
    body(body) {}

    request::request(method _method, std::string resource): _method(_method), resource(resource),
    _version(version::HTTP_2_0) {
        this->set_header("Content-Length", "0");
    }

    request::request() {
        this->set_header("Content-Length", "0");
    }

    std::string request::serialize() {
        std::string str;
        str += to_string(_method);
        str += SPACE;
        str += this->resource;
        str += SPACE;
        str += to_string(_version);
        str += CRLF;

        for(auto &p : headers) {
            str += p.second.serialize() + CRLF;
        }

        str += CRLF;

        str += body;

        return str;
    }

    void request::set_method(method m) {
        this->_method = m;
    }

    void request::set_resource(std::string res) {
        this->resource = res;
    }

    void request::set_version(version v) {
        this->_version = v;
    }

    void request::set_header(std::string key, std::string value) {
        headers[key] = header(key, value);
    }

    void request::set_body(std::string &body) {
        this->body = body;
        this->set_header("Content-Length", std::to_string(body.length()));
    }

    void request::remove_header(std::string key) {
        if(headers.find(key) != headers.end())
            headers.erase(key);
    }

    method request::get_method() {
        return _method;
    }

    version request::get_version() {
        return _version;
    }

    std::string request::get_resource() {
        return resource;
    }

    std::map<std::string, header> request::get_header_map() {
        return headers;
    }

    std::string request::get_header(std::string key) {
        if(headers.find(key) == headers.end()) {
            throw no_such_header_exception();
        }

        return headers[key].value;
    }

    std::string request::get_body() {
        return body;
    }
} // http