#ifndef _USER_HPP_
#define _USER_HPP_

#include <iostream>
class User
{
private:
    std::string username;
    std::string id;

public:
    User(std::string username, std::string id);
    std::string get_username() const;
    std::string get_id() const;

    friend std::ostream &operator<<(std::ostream &os, const User &user);
};

#endif