//
// Created by abhishek on 25/9/22.
//

#ifndef ISJ_RE_22_14571_RESPONSE_H
#define ISJ_RE_22_14571_RESPONSE_H
#include "header.h"
#include "http_exception.h"
#include <map>

namespace http {

    enum class status {
        OK = 200,
        NOT_FOUND = 404,
        BAD_REQUEST = 400,
        UNAUTHORIZED = 401,
        FORBIDDEN = 403,
        METHOD_NOT_ALLOWED = 405,
        CONFLICT = 409,
        LENGTH_REQ = 411,
        INTERNAL_SERVER_ERROR = 500,
        UNDEFINED = 1000
    };

    status to_status(std::string code);

    std::string to_string(status code);

    /*
        response
    */
    class response {
        version _version;
        status _status;
        std::string status_txt;
        std::map <std::string, header> headers;
        std::string body;

    public:
        response(version _version,
                 status _status,
                 std::string status_txt,
                 const std::map <std::string, header> &headers,
                 std::string body);

        response();

        response(status _status, std::string status_txt);

        std::string serialize();

        static response deserialize(std::string res) {
            try {
                int pos = res.find(CRLF + CRLF);
                std::string res_head = res.substr(0, pos + 1);
                std::string body = res.substr(pos + 4);

                /* deserialize header */
                std::vector <std::string> lines = tb_util::tokenize(res_head, CRLF);
                std::vector <std::string> segs = tb_util::tokenize(lines[0], SPACE);
                version _version = to_version(segs.at(0));
                status _status = to_status(segs.at(1));
                std::string status_txt = "";
                for (int i = 2; i < segs.size(); ++i)
                    status_txt += " " + segs.at(i);
                status_txt = status_txt.substr(1);


                std::map <std::string, header> headers;
                for (int i = 1; i < lines.size(); ++i) {
                    header h = header::deserialize(lines.at(i));
                    headers[h.key] = h;
                }

                return response(_version, _status, status_txt, headers, body);
            } catch (std::exception e) {
                std::string msg = e.what();
                throw parse_exception("Failed to parse request: " + msg);
            }
        }

        version get_version();

        status get_status();

        std::string get_status_text();

        std::map <std::string, header> get_header_map();

        std::string get_header(std::string key);

        std::string get_body();

        void set_version(version v);

        void set_status(status s);

        void set_status_text(std::string text);

        void add_header(std::string key, std::string value);

        void remove_header(std::string key);

        void set_body(std::string body);
    };
}

#endif //ISJ_RE_22_14571_RESPONSE_H
