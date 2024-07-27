#ifndef _SERVER_MANAGER_HPP_
#define _SERVER_MANAGER_HPP_

#include <thread>
#include <map>
#include <pqxx/pqxx>
#include <memory>
#include <optional>

#include "server_controller.hpp"
#include "db_controller.hpp"

class ServerManager
{
public:
    ServerManager();
    ~ServerManager();

    void create_server(std::string name, std::string owner);
    void close(std::optional<std::string> server_name = std::nullopt);
    void display_log(std::string server_name);
    void get_users_in_server(std::string server_name);

    std::map<std::string, int> get_all_servers();
    std::map<std::string, int> get_servers();

private:
    struct ServerData
    {
        std::thread thread;
        std::shared_ptr<ServerController> controller;
    };

    std::map<std::string, ServerData> servers;
    DBController db_controller;
    static std::atomic<int> next_port;

    static int get_next_port();
    void destroy_server(std::string name);
};

#endif
