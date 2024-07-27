#ifndef _SERVER_CONTROLLER_HPP_
#define _SERVER_CONTROLLER_HPP_

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <set>
#include <map>
#include <atomic>
#include <mutex>
#include <fstream>

typedef websocketpp::server<websocketpp::config::asio> server;

class ServerController
{
public:
    ServerController(const int port);
    void run_server();
    void shut_down_server(std::thread &server_thread);
    void read_log();
    void display_users();
    int get_port() const;
    std::string get_name() const;

private:
    std::atomic<bool> running;
    const std::string name;
    const int port;
    std::string log_filename;
    std::ofstream log_file;
    std::mutex log_mutex;
    server _server;

    struct connection_hdl_comp
    {
        bool operator()(const websocketpp::connection_hdl &lhs, const websocketpp::connection_hdl &rhs) const
        {
            return std::owner_less<websocketpp::connection_hdl>()(lhs, rhs);
        }
    };
    std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> active_connections;
    std::map<websocketpp::connection_hdl, std::string, connection_hdl_comp> clients;

    void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg);
    void on_close_server(websocketpp::connection_hdl hdl);
    void on_open(websocketpp::connection_hdl hdl);
    void log(const std::string &message);
};

#endif