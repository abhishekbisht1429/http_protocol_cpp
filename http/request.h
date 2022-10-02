//
// Created by abhishek on 25/9/22.
//

#ifndef ISJ_RE_22_14571_REQUEST_H
#define ISJ_RE_22_14571_REQUEST_H
#include "header.h"
#include "http_exception.h"
#include <map>

namespace http {

    class request {
        method _method;
        version _version;
        std::string resource;
        std::map<std::string, header> headers;
        std::string body;

    public:
        request(method _method, std::string resource, version _version,
                std::map<std::string, header> &headers, std::string body);
        request(method _method, std::string resource);
        request();

        std::string serialize();

        static request deserialize(std::string req) {
            try {
                int pos = req.find(CRLF + CRLF);
                std::string req_head = req.substr(0, pos + 1);
                std::string body = req.substr(pos + 4);
//TODO: fix issues in line 37 - when number of keywords is large, there is some issue. use string instead of bytes
// first.
                /* deserialize header */
                std::vector<std::string> lines = tb_util::tokenize(req_head, CRLF);
                std::vector<std::string> segs = tb_util::tokenize(lines[0], SPACE);
                method m = to_method(segs[0]);
                std::string resource = segs[1];
                version v = to_version(segs[2]);

                std::map<std::string, header> headers;
                for(int i=1; i<lines.size(); ++i) {
                    header h = header::deserialize(lines[i]);
                    headers[h.key] = h;
                }

                return request(m, resource, v, headers, body);
            } catch(std::exception e) {
                std::string msg = e.what();
                throw parse_exception("Failed to parse request: " + msg);
            }
        }

        void set_method(method m);

        void set_resource(std::string res);

        void set_version(version v);

        void set_header(std::string key, std::string value);

        void set_body(std::string &body);

        void remove_header(std::string key);

        method get_method();

        version get_version();

        std::string get_resource();

        std::map<std::string, header> get_header_map();

        std::string get_header(std::string key);

        std::string get_body();
    };

} // http

#endif //ISJ_RE_22_14571_REQUEST_H
