#ifndef CLIENT_CONTROLLER_HPP
#define CLIENT_CONTROLLER_HPP

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <ncurses.h>
#include <vector>
#include <mutex>

#include "db_controller.hpp"

class ClientController
{
public:
    ClientController();
    ~ClientController();

    void join_server(std::string username, std::string user_id, int port);
    void cleanup();

private:
    DBController db_controller;
    websocketpp::client<websocketpp::config::asio_client> websocket_client;
    std::vector<std::string> messages;
    std::mutex messages_mutex;

    bool running;
    std::string _user_id;
    std::string _server_id;
    WINDOW *message_win;
    WINDOW *input_win;

    void set_up(int port);
    void on_message(websocketpp::connection_hdl hdl, websocketpp::config::asio_client::message_type::ptr msg);
    void display_messages();
    void handle_input(websocketpp::connection_hdl con);
};

#endif
