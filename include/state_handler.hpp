#ifndef _STATE_HANDLER_HPP_
#define _STATE_HANDLER_HPP_

#include "states.hpp"
#include "server_manager.hpp"
// #include "server_controller.hpp"
#include "client_controller.hpp"
#include "user_controller.hpp"

class StateHandler
{
private:
    UserController user_controller;
    ClientController client_controller;
    // ServerController server_controller;
    ServerManager server_manager;

public:
    State auth_page();
    State login_page();
    State register_page();
    State menu_page();
    State join_server_page();
    State create_server_page();
};

#endif