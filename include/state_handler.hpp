#ifndef _STATE_HANDLER_HPP_
#define _STATE_HANDLER_HPP_

#include <optional>

#include "client_controller.hpp"
#include "user_controller.hpp"
#include "server_manager.hpp"
#include "states.hpp"
#include "user.hpp"

class StateHandler
{
private:
    UserController user_controller;
    ClientController client_controller;
    ServerManager server_manager;
    std::optional<User> user;

public:
    State auth_page();
    State login_page();
    State register_page();
    State menu_page();
    State join_server_page();
    State create_server_page();
    State view_servers_page();
    void server_log_page(std::string server_name);
    void manage_server_page(std::string server_name);
    void quit();
};

#endif