#include <iostream>

#include "user.hpp"

User::User(std::string username) : username(username){};

std::string User::get_username() const
{
    return username;
};

std::ostream &operator<<(std::ostream &os, const User &user)
{
    os << "User(username: " << user.get_username() << ")";
    return os;
}