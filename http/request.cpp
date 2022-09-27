//
// Created by abhishek on 25/9/22.
//

#include "request.h"

namespace http {
    void request::init() {
        this->set_header("Content-Length", "0");
    }
    request::request(method _method, std::string resource, version _version,
                     std::map<std::string, header> &headers, tb_util::bytes body):
    _method(_method), resource(resource), headers(headers), _version(_version),
    body(body) {
        init();
    }

    request::request(method _method, std::string resource): _method(_method), resource(resource),
    _version(version::HTTP_2_0) {
        init();
    }

    request::request() {
        init();
    }

    tb_util::bytes request::serialize() {
        tb_util::bytes bstr;
        bstr += tb_util::s2b(to_string(_method));
        bstr += tb_util::s2b(SPACE);
        bstr += tb_util::s2b(this->resource);
        bstr += tb_util::s2b(SPACE);
        bstr += tb_util::s2b(to_string(_version));
        bstr += tb_util::s2b(CRLF);

        for(auto &p : headers) {
            bstr += tb_util::s2b(p.second.serialize() + CRLF);
        }

        bstr += tb_util::s2b(CRLF);

        bstr += body;

        return bstr;
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

    void request::set_body(tb_util::bytes &body) {
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

    tb_util::bytes request::get_body() {
        return body;
    }
} // http