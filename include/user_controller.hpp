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
    User create_user(std::string username, std::string password);
    bool validate_user(std::string username, std::string entered_password);
    std::string get_user_id(std::string username);
    void set_as_active(std::string username);
    void unset_as_active(std::string username);
};

#endif