#include "server_manager.hpp"
#include "server_controller.hpp"

ServerManager::ServerManager(){};

void ServerManager::create_server(std::string name, std::string owner)
{
    std::string port = "9002";
    std::string user_name = "sindri";
    std::string get_user_query = "SELECT id from custom_user where username = '" + user_name + "';";
    pqxx::result user_result = db_controller.execute_query(get_user_query);
    std::string user_id = user_result[0]["id"].as<std::string>();

    std::string query = "INSERT INTO server (name, port, owner) VALUES ('" + name + "', '" + port + "', '" + user_id + "')";
    db_controller.execute_query(query);

    std::shared_ptr<ServerController> server_controller = std::make_shared<ServerController>();
    std::thread run_server_thread(&ServerController::run_server, server_controller);
    ServerData server_data;
    server_data.thread = std::move(run_server_thread);
    server_data.controller = server_controller;

    servers[name] = std::move(server_data);
};

void ServerManager::destroy_server(std::string name)
{
    auto server_data_it = servers.find(name);
    ServerData &server_data = server_data_it->second;
    std::thread &server_thread = server_data.thread;

    server_data.controller->shut_down_server(server_thread);

    servers.erase(server_data_it);
    std::string query = "DELETE FROM server WHERE name = '" + name + "';";
    db_controller.execute_query(query);
};

pqxx::result ServerManager::fetch_servers(std::string user_id)
{
    std::string query = "SELECT * FROM server where owner = '" + user_id + "';";
    return db_controller.execute_query(query);
};

void ServerManager::close_all(std::string user_id)
{
    pqxx::result servers = fetch_servers(user_id);

    for (const auto &row : servers)
    {
        std::string server_name = row["name"].as<std::string>();
        destroy_server(server_name);
    }
}