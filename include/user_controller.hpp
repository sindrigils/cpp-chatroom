#ifndef _USER_CONTROLLER_HPP_
#define _USER_CONTROLLER_HPP_

#include "user.hpp"
#include "db_controller.hpp"

class UserController
{
private:
    DBController db_controller;

public:
    User create_user(std::string username, std::string password);
    std::vector<User> get_all_users();
    std::string get_user_id(std::string username);
    bool validate_user(std::string username, std::string entered_password);
    void set_as_active(std::string username);
    void unset_as_active(std::string username);
};

#endif