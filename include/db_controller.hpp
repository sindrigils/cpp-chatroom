#ifndef _DB_CONTROLLER_HPP
#define _DB_CONTROLLER_HPP

#include <iostream>
#include <pqxx/pqxx>
#include <cstdlib>

static inline std::string get_env_var(const std::string &key, const std::string &default_value)
{
    const char *val = std::getenv(key.c_str());
    return val == nullptr ? default_value : std::string(val);
}

class DBController
{
public:
    DBController(std::string dbname = "cpp-chatroom",
                 std::string user = "postgres",
                 std::string password = "",
                 std::string host_address = "127.0.0.1",
                 std::string port = "5433");
    pqxx::connection get_connection();
    pqxx::result execute_query(std::string query);

private:
    std::string dbname;
    std::string user;
    std::string password;
    std::string host_address;
    std::string port;
};

#endif