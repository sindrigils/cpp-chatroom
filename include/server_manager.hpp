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
    void create_server(std::string name, std::string owner);
    void close(std::optional<std::string> server_name = std::nullopt);
    void display_log(std::string server_name);
    std::map<std::string, int> get_all_servers();
    std::map<std::string, int> get_servers();

private:
    static int get_next_port();
    static std::atomic<int> next_port;
    struct ServerData
    {
        std::thread thread;
        std::shared_ptr<ServerController> controller;
    };
    std::map<std::string, ServerData> servers;
    DBController db_controller;
    void destroy_server(std::string name);
};

#endif
