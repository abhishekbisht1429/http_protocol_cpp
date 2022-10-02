//
// Created by abhishek on 25/9/22.
//

#include "../net_socket/socket_exception.h"
#include "http_client.h"
#include <iostream>
#include <functional>
#include <thread>

namespace http {
    response http_client::read_response() {
        /* read response from server */
        std::string msg_header, msg_body;
        int pos_hend;
        char buf[1024];
        int read_count = 0;
        std::cout<<"reading bytes from socket...\n";
        /* reading header part */
        while((read_count = sock.read_bytes(buf, 1024)) > 0) {
//            std::cout<<"read count: "<<read_count<<"\n";
            std::string temp;
            for(int i=0; i<read_count; ++i)
                temp += buf[i];

            /* find end of header */
            pos_hend = temp.find(CRLF + CRLF);
            if(pos_hend != std::string::npos) {
                /* end of request header found */
                msg_header += temp.substr(0, pos_hend);
                msg_header += CRLF + CRLF;

                msg_body += temp.substr(pos_hend+4);
                break;
            }
            msg_header += temp;
        }
        if(read_count == 0 && msg_header.size() == 0)
            throw remote_end_closed_exception();

        /* parsing request/response header */
//        std::cout<<"parsing header\n";
//        std::cout << "message_header:\n-----------------------\n";
//        std::cout<<msg_header;
//        std::cout<<"\n-----------------------\n";
        http::response res = http::response::deserialize(msg_header);
        /* get Content-Length */
        int len = 0;
        try {
            len = std::stol(res.get_header("Content-Length"));
        } catch(no_such_header_exception nshe) {
            std::cout<<nshe.what()<<"\n";
        }
        len -= msg_body.size();
        len = std::max(0, len);

        /* read rest of the body */
        while(len > 0) {
            read_count = sock.read_bytes(buf, 1024);
            for(int i=0; i<read_count; ++i)
                msg_body += buf[i];
            len -= read_count;
        }
        res.set_body(msg_body);
        return res;
    }

    void http_client::write_request(request req) {
        std::string data = req.serialize();
        // std::cout<<"data:\n"<<b2s(data)<<"\n";
        int to_send = data.size();
        std::cout<<"Number of bytes to send: "<<to_send<<"\n";
        while(to_send>0) {
            to_send -= sock.write_bytes((void*)data.c_str(), to_send);
        }
        std::cout<<"client sent request\n";
    }

    void http_client::reconnect() {
        sock.connect_server(server_sockaddr);
    }

    http_client::http_client() {}

    void http_client::connect(net_socket::sock_address server_addr) {
        this->server_sockaddr = server_addr;
        try {
            sock.connect_server(server_sockaddr);
        } catch(net_socket::is_connected_exception ice) {
            /* ignore if already connected */
        }
    }
    void http_client::connect(net_socket::ipv4_address ip, uint16_t port) {
        connect(net_socket::sock_address(ip, port));
    }

    void http_client::disconnect() {
        sock.close_socket();
    }

    response http_client::send_request(request req) {
        std::cout<<"client sending request\n";
        int retry_count = 0;
        while(1) {
            try {
                write_request(req);
                std::cout<<"\n--------------Waiting for response------------\n";
                auto res = read_response();
                std::cout<<"\n--------------Response from server------------\n";
                std::cout<<http::to_string(res.get_status())<<": "<<res.get_status_text()<<"\n";
                // std::cout<<b2s(res.serialize())<<"\n";
                return res;
            } catch(net_socket::sock_not_connected_exception snce) {
                if(retry_count > 5)
                    throw snce;
                reconnect();
                ++retry_count;
            } catch(net_socket::broken_pipe_exception bpe) {
                if(retry_count > 5)
                    throw bpe;
                reconnect();
                ++retry_count;
            } catch(net_socket::conn_reset_exception cre) {
                if(retry_count > 5)
                    throw cre;
                reconnect();
                ++retry_count;
            }
        }
    }



    template<typename Callback, typename... Args>
    void http_client::send_request_async(request req, Callback callback, Args... args) {
        std::thread t(f<Callback>, std::ref(*this), std::ref(req), std::ref(callback, args...));
        t.join();
    }
} // http