//
// Created by abhishek on 25/9/22.
//

#ifndef HTTP_PROTOCOL_CPP_REQUEST_H
#define HTTP_PROTOCOL_CPP_REQUEST_H
#include "header.h"
#include "http_exception.h"

namespace http {

    class request {
        method _method;
        version _version;
        std::string resource;
        std::map<std::string, header> headers;
        tb_util::bytes body;

        void init();
    public:
        request(method _method, std::string resource, version _version,
                std::map<std::string, header> &headers, tb_util::bytes body);
        request(method _method, std::string resource);
        request();

        tb_util::bytes serialize();

        static request deserialize(tb_util::bytes req) {
            try {
                int pos = req.find(tb_util::s2b(CRLF + CRLF));
                std::string req_head = tb_util::b2s(req.substr(0, pos + 1));
                tb_util::bytes body = req.substr(pos + 4);

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

        void set_body(tb_util::bytes &body);

        void remove_header(std::string key);

        method get_method();

        version get_version();

        std::string get_resource();

        std::map<std::string, header> get_header_map();

        std::string get_header(std::string key);

        tb_util::bytes get_body();
    };

} // http

#endif //HTTP_PROTOCOL_CPP_REQUEST_H
