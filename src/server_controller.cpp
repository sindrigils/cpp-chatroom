#include <iostream>

#include "server_controller.hpp"

std::atomic<int> ServerController::next_port{9002};

ServerController::ServerController(const std::string &name) : running(true), name(name)
{
    std::string log_filename = "logs/log_" + name + ".txt";
    log_file.open(log_filename);
    _server.set_access_channels(websocketpp::log::alevel::all);
    _server.clear_access_channels(websocketpp::log::alevel::frame_payload);
    _server.set_error_channels(websocketpp::log::elevel::all);

    _server.get_alog().set_ostream(&log_file);
    _server.get_elog().set_ostream(&log_file);
};
void ServerController::run_server()
{
    int port = get_next_port();

    _server.set_open_handler(bind(&ServerController::on_open, this, std::placeholders::_1));
    _server.set_close_handler(bind(&ServerController::on_close_server, this, std::placeholders::_1));
    _server.set_message_handler(bind(&ServerController::on_message, this, std::placeholders::_1, std::placeholders::_2));

    // initlaize the event loop, allowing async operations. This allows the server to handle events in async such as new connections, sending message and etc.
    _server.init_asio();
    _server.listen(port);
    _server.start_accept();

    log("WebSocket server listening on port " + std::to_string(port) + "...");

    try
    {
        while (running)
        {
            _server.poll();
            // add a small delay to prevent a tight loop
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Excpetion in try catch" << std::endl;
    }
};

void ServerController::shut_down_server(std::thread &server_thread)
{
    log("Server closed");
    running = false;
    server_thread.join();

    active_connections.clear();
    clients.clear();
    log_file.close();
};

void ServerController::on_open(websocketpp::connection_hdl hdl)
{
    _server.send(hdl, "Welcome to the server", websocketpp::frame::opcode::text);
    active_connections.insert(hdl);
};

void ServerController::on_close_server(websocketpp::connection_hdl hdl)
{
    log("Connection closed");

    active_connections.erase(hdl);
}

void ServerController::on_message(websocketpp::connection_hdl hdl, server::message_ptr msg)

{
    auto message = msg->get_payload();

    auto it = clients.find(hdl);
    if (it == clients.end())
    {
        log("Adding user with name: " + message + " to the map");

        clients[hdl] = message;
        return;
    }

    log("Received message: " + message);

    std::string sender_name = it->second;
    std::string broadcast_msg = sender_name + ": " + message;

    for (auto it : active_connections)
    {
        if (it.lock() != hdl.lock())
        {
            _server.send(it, broadcast_msg, websocketpp::frame::opcode::text);
        }
    }
};

int ServerController::get_next_port()
{
    return next_port++;
}

void ServerController::log(const std::string &message)
{
    if (log_file.is_open())
    {
        log_file << message << std::endl;
    }
}