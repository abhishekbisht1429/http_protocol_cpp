//
// Created by abhishek on 25/9/22.
//

#include "response.h"

namespace http {
    status to_status(std::string code) {
        if (code == "200")
            return status::OK;
        else if (code == "404")
            return status::NOT_FOUND;
        else if (code == "400")
            return status::BAD_REQUEST;
        else if (code == "401")
            return status::UNAUTHORIZED;
        else if (code == "403")
            return status::FORBIDDEN;
        else if (code == "405")
            return status::METHOD_NOT_ALLOWED;
        else if (code == "409")
            return status::CONFLICT;
        else if (code == "411")
            return status::LENGTH_REQ;
        else if (code == "500")
            return status::INTERNAL_SERVER_ERROR;
        else
            return status::UNDEFINED;
    }

    std::string to_string(status code) {
        switch (code) {
            case status::OK:
                return "200";
            case status::BAD_REQUEST:
                return "400";
            case status::UNAUTHORIZED:
                return "401";
            case status::FORBIDDEN:
                return "403";
            case status::METHOD_NOT_ALLOWED:
                return "405";
            case status::CONFLICT:
                return "409";
            case status::NOT_FOUND:
                return "404";
            case status::LENGTH_REQ:
                return "411";
            case status::INTERNAL_SERVER_ERROR:
                return "500";
            default:
                return "1000";
        }
    }

    response::response(version _version,
                       status _status,
                       std::string status_txt,
                       const std::map <std::string, header> &headers,
                       tb_util::bytes body) :
            _version(_version), _status(_status),
            status_txt(status_txt), headers(headers), body(body) {}

    response::response() {}

    response::response(status _status, std::string status_txt) {
        this->_version = version::HTTP_2_0;
        this->_status = _status;
        this->status_txt = status_txt;
        add_header("Content-Length", "0");
    }

    tb_util::bytes response::serialize() {
        tb_util::bytes bstr;
        bstr += tb_util::s2b(to_string(_version));
        bstr += tb_util::s2b(SPACE);
        bstr += tb_util::s2b(to_string(_status));
        bstr += tb_util::s2b(SPACE);
        bstr += tb_util::s2b(status_txt);
        bstr += tb_util::s2b(CRLF);

        for (auto &p: headers) {
            bstr += tb_util::s2b(p.second.serialize());
            bstr += tb_util::s2b(CRLF);
        }

        bstr += tb_util::s2b(CRLF);
        bstr += body;

        return bstr;
    }

    version response::get_version() {
        return _version;
    }

    status response::get_status() {
        return _status;
    }

    std::string response::get_status_text() {
        return status_txt;
    }

    std::map <std::string, header> response::get_header_map() {
        return headers;
    }

    std::string response::get_header(std::string key) {
        if (headers.find(key) == headers.end()) {
            throw no_such_header_exception();
        }

        return headers[key].value;
    }

    tb_util::bytes response::get_body() {
        return body;
    }

    void response::set_version(version v) {
        this->_version = v;
    }

    void response::set_status(status s) {
        this->_status = s;
    }

    void response::set_status_text(std::string text) {
        this->status_txt = text;
    }

    void response::add_header(std::string key, std::string value) {
        headers[key] = header(key, value);
    }

    void response::remove_header(std::string key) {
        if (headers.find(key) != headers.end())
            headers.erase(key);
    }

    void response::set_body(tb_util::bytes body) {
        this->body = body;
    }
}