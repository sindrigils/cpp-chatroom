#ifndef _USER_CONTROLLER_HPP_
#define _USER_CONTROLLER_HPP_

#include "user.hpp"
#include "db_controller.hpp"

class UserController
{
private:
    DBController db_controller;

public:
    std::vector<User> get_all_users();
    std::unique_ptr<User> create_user(std::string username, std::string password);
    bool validate_user(std::string username, std::string entered_password);
};

#endif