#ifndef _USER_HPP_
#define _USER_HPP_

#include <iostream>
class User
{
private:
    std::string username;

public:
    User(std::string username);
    std::string get_username() const;

    friend std::ostream &operator<<(std::ostream &os, const User &user);
};

#endif