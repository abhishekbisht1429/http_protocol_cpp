#include "http_server.h"
namespace http {

    request http_server::read_request(net_socket::inet_socket &sock) {
        /* read request from cient */
        request req;
        char buf[1024];
        int pos_hend;
        int read_count;
        std::string msg_header, msg_body;

        //TODO: Later find a way for reading request with very large body

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

        /* parsing request header */
//        std::cout<<"parsing header\n";
        req = http::request::deserialize(msg_header);
        /* check if Content-Length is available */
        int len = 0;
        try {
            len = std::stol(req.get_header("Content-Length"));
        } catch(no_such_header_exception nshe) {
            throw content_length_missing_exception();
        }

        len -= msg_body.size();

        /* read rest of the body */
        while(len > 0) {
            read_count = sock.read_bytes(buf, 1024);
            for(int i=0; i<read_count; ++i)
                msg_body += buf[i];
            len -= read_count;
        }
        req.set_body(msg_body);
        return req;
    }

    void http_server::write_response(response res, net_socket::inet_socket &sock) {
        std::string data = res.serialize();
        // std::cout<<"data:\n"<<b2s(data)<<"\n";
        int to_send = data.size();
        std::cout<<"Number of bytes to send: "<<to_send<<"\n";
        while(to_send>0) {
            to_send -= sock.write_bytes((void*)data.c_str(), to_send);
        }
    }

    http_server::http_server(net_socket::ipv4_address ip, uint16_t port) {
        try {
            server_sockaddr = net_socket::sock_address(ip, port);
        } catch (std::exception e) {
            std::cout<<e.what()<<"\n";
            throw http_exception("http_server: failed to create net_socket");
        }
    }

    http_server::http_server() {}

    // template<typename Callback>
    // static void handle_client(net_socket::inet_socket &sock, Callback &callback, http_server &server) {

    void http_server::serve(std::function<response(request, net_socket::sock_address)> callback) {
        try {
            server_sock.bind_address(server_sockaddr.ip,server_sockaddr.port);
            server_sock.sock_listen(MAX_CONN);
            std::vector<std::thread> threads;
            while(1) {
                std::cout<<"Main Thread "<<std::this_thread::get_id()<<": waiting for new connection\n";
                net_socket::inet_socket sock = server_sock.accept_connection();
                std::cout<<"Main Thread "<<std::this_thread::get_id()<<": received a connection\n";
                std::cout<<"Main Thread "<<std::this_thread::get_id()<<": delegating connection to worker thread...\n";

                /* create a seperate thread to handle the client */
                threads.push_back(std::thread(handle_client,
                                                std::ref<net_socket::inet_socket>(sock),
                                                std::ref<std::function<response(request, net_socket::sock_address)>>(callback),
                                                std::ref(*this)));
            }
            for(int i=0; i<threads.size(); ++i)
                if(threads[i].joinable())
                    threads[i].join();
            std::cout<<std::this_thread::get_id()<<" exiting\n";
        } catch(net_socket::socket_exception se) {
            std::cout<<se.what()<<"\n";
            throw se;
        }catch(std::exception &e) {
            std::cout<<e.what()<<"\n";
            throw e;
        }
    }

    void http_server::stop() {
        server_sock.close_socket();
    }
};