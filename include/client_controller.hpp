#ifndef CLIENT_CONTROLLER_HPP
#define CLIENT_CONTROLLER_HPP

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <vector>
#include <mutex>
#include <ncurses.h>

class ClientController
{
public:
    ClientController();
    ~ClientController();

    void join_server();
    void cleanup();

private:
    websocketpp::client<websocketpp::config::asio_client> websocket_client;
    std::vector<std::string> messages;
    std::mutex messages_mutex;
    bool running;

    void set_up();
    void on_message(websocketpp::connection_hdl hdl, websocketpp::config::asio_client::message_type::ptr msg);
    void display_messages();
    void handle_input(websocketpp::connection_hdl con);

    WINDOW *message_win;
    WINDOW *input_win;
};

#endif
