#include <iostream>

#include "user.hpp"

User::User(std::string username, std::string id) : username(username), id(id){};

std::string User::get_username() const
{
    return username;
};

std::string User::get_id() const
{
    return id;
};

std::ostream &operator<<(std::ostream &os, const User &user)
{
    os << "User(username: " << user.get_username() << ")";
    return os;
}