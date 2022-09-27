//
// Created by abhishek on 25/9/22.
//

#ifndef ISJ_RE_22_14571_HTTP_SERVER_H
#define ISJ_RE_22_14571_HTTP_SERVER_H
#include "../net_socket/sock_address.h"
#include "request.h"
#include "response.h"
#include "../net_socket/inet_server_socket.h"
#include "../net_socket/socket_exception.h"
namespace http {
    class http_server {
        constexpr static int MAX_CONN = 10;
        net_socket::sock_address server_sockaddr;
        net_socket::inet_server_socket server_sock;

        request read_request(net_socket::inet_socket &sock);

        void write_response(response res, net_socket::inet_socket &sock);

    public:
        http_server(net_socket::ipv4_address ip, uint16_t port);

        http_server();

        // template<typename Callback>
        // static void handle_client(net_socket::inet_socket &sock, Callback &callback, http_server &server) {
        static void handle_client(net_socket::inet_socket sock,
                                               std::function<response(request, net_socket::sock_address)> callback,
                                               http_server &server) {
            std::cout<<"http_server: handling client by "<<std::this_thread::get_id()<<"\n";

            while(1) {
                try {
                    http::request req = server.read_request(sock);
                    std::cout<<"client request data: \n";
                    std::cout << tb_util::b2s(req.serialize()) << "\n";
                    std::cout<<"calling callback\n";
                    server.write_response(callback(req,
                                                   sock.get_remote_address()), sock);
                } catch(remote_end_closed_exception &rece) {
                    std::cout<<rece.what()<<"\n";
                    break;
                } catch(net_socket::socket_time_out_exception &stoe) {
                    std::cout<<stoe.what()<<"\n";
                    break;
                } catch(content_length_missing_exception clme) {
                    server.write_response(response(status::LENGTH_REQ, "Content-Length missing"), sock);
                } catch(http_exception &he) {
                    std::cout<<he.what()<<"\n";
                    break;
                }catch(parse_exception &pe) {
                    std::cout<<pe.what()<<"\n";
                    server.write_response(response(status::BAD_REQUEST, "Malformed HTTP request"), sock);
                } catch(std::exception &e) {
                    std::cout<<"http_client: unexpected exception\n";
                    std::cout<<e.what()<<"\n";
                    break;
                }
            }
            sock.close_socket();

            std::cout<<"http_server: client request handled\n";
            std::cout<<std::this_thread::get_id()<<" exiting\n";
        }

        void serve(std::function<response(request, net_socket::sock_address)> callback);

        void stop();

    };
}
#endif //ISJ_RE_22_14571_HTTP_SERVER_H
