#ifndef _SERVER_MANAGER_HPP_
#define _SERVER_MANAGER_HPP_

#include <thread>
#include <map>
#include <pqxx/pqxx>
#include <memory>

#include "server_controller.hpp"
#include "db_controller.hpp"

class ServerManager
{
public:
    ServerManager();
    void create_server(std::string name, std::string owner);
    void destroy_server(std::string name);
    pqxx::row fetch_servers();

private:
    struct ServerData
    {
        std::thread thread;
        std::shared_ptr<ServerController> controller;
    };
    std::map<std::string, ServerData> servers;
    DBController db_controller;
};

#endif
