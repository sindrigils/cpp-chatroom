#include <iostream>

#include "server_controller.hpp"

std::atomic<int> ServerController::next_port{9002};

ServerController::ServerController() : running(true){};
void ServerController::run_server()
{
    int port = get_next_port();

    _server.set_open_handler(bind(&ServerController::on_open, this, std::placeholders::_1));
    _server.set_close_handler(bind(&ServerController::on_close_server, this, std::placeholders::_1));
    _server.set_message_handler(bind(&ServerController::on_message, this, std::placeholders::_1, std::placeholders::_2));

    // initlaize the event loop, allowing async operations. This allows the server to handle events in async such as new connections, sending message and etc.
    _server.init_asio();

    // bind the server to this port, the server will listen for TCP connections on this port.
    _server.listen(port);

    // start the acceptance loop, now the server will accept new connections.
    _server.start_accept();

    std::cout << "WebSocket server listening on port " << port << "..." << std::endl;

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
    std::cout << "Server closed" << std::endl;
    running = false;
    server_thread.join();

    active_connections.clear();
    clients.clear();
};

void ServerController::on_open(websocketpp::connection_hdl hdl)
{
    _server.send(hdl, "Welcome to the server", websocketpp::frame::opcode::text);
    active_connections.insert(hdl);
};

void ServerController::on_close_server(websocketpp::connection_hdl hdl)
{
    std::cout << "Connection closed" << std::endl;
    active_connections.erase(hdl);
}

void ServerController::on_message(websocketpp::connection_hdl hdl, server::message_ptr msg)

{
    auto message = msg->get_payload();

    auto it = clients.find(hdl);
    if (it == clients.end())
    {
        std::cout << "Adding user with name: " << message << " to the map" << std::endl;
        clients[hdl] = message;
        return;
    }

    std::cout << "Received message: " << message << std::endl;

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