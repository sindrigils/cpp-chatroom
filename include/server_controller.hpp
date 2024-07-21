#ifndef _SERVER_CONTROLLER_HPP_
#define _SERVER_CONTROLLER_HPP_

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <set>
#include <map>
#include <atomic>
#include <fstream>

typedef websocketpp::server<websocketpp::config::asio> server;

class ServerController
{
public:
    ServerController(const std::string &name);
    void run_server();
    void shut_down_server(std::thread &server_thread);
    std::string get_name() const;

private:
    struct connection_hdl_comp
    {
        bool operator()(const websocketpp::connection_hdl &lhs, const websocketpp::connection_hdl &rhs) const
        {
            return std::owner_less<websocketpp::connection_hdl>()(lhs, rhs);
        }
    };
    server _server;
    std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> active_connections;
    std::map<websocketpp::connection_hdl, std::string, connection_hdl_comp> clients;

    void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg);
    void on_open(websocketpp::connection_hdl hdl);
    void on_close_server(websocketpp::connection_hdl hdl);
    static int get_next_port();
    static std::atomic<int> next_port;
    std::atomic<bool> running;
    const std::string name;
    std::ofstream log_file;

    void log(const std::string &message);
};

#endif