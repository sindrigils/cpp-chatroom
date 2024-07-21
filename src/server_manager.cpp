#include "server_manager.hpp"
#include "server_controller.hpp"

std::atomic<int> ServerManager::next_port{9002};

ServerManager::ServerManager(){};

void ServerManager::create_server(std::string name, std::string owner)
{
    int port = get_next_port();
    std::string user_name = "sindri";
    std::string get_user_query = "SELECT id from custom_user where username = '" + user_name + "';";
    pqxx::result user_result = db_controller.execute_query(get_user_query);
    std::string user_id = user_result[0]["id"].as<std::string>();

    std::string query = "INSERT INTO server (name, port, owner) VALUES ('" + name + "', '" + std::to_string(port) + "', '" + user_id + "')";
    db_controller.execute_query(query);

    std::shared_ptr<ServerController> server_controller = std::make_shared<ServerController>(port);
    std::thread run_server_thread(&ServerController::run_server, server_controller);
    ServerData server_data;
    server_data.thread = std::move(run_server_thread);
    server_data.controller = server_controller;

    servers[name] = std::move(server_data);
};

std::map<std::string, int> ServerManager::get_all_servers()
{
    std::map<std::string, int> ret_servers;
    std::string query = "SELECT name, port from server";
    pqxx::result result = db_controller.execute_query(query);
    for (const auto &row : result)
    {
        ret_servers.emplace(row["name"].as<std::string>(), row["port"].as<int>());
    }
    return ret_servers;
}

std::map<std::string, int> ServerManager::get_servers()
{
    std::map<std::string, int> ret_servers;
    for (const auto &pair : servers)
    {
        ret_servers.emplace(pair.first, pair.second.controller->get_port());
    }
    return ret_servers;
};

void ServerManager::destroy_server(std::string name)
{
    auto server_data_it = servers.find(name);
    ServerData &server_data = server_data_it->second;
    std::thread &server_thread = server_data.thread;

    server_data.controller->shut_down_server(server_thread);
    std::string query = "DELETE FROM server WHERE name = '" + name + "';";
    db_controller.execute_query(query);
};

void ServerManager::close(std::optional<std::string> server_name)
{
    if (server_name)
    {
        destroy_server(*server_name);
        servers.erase(*server_name);
    }
    else
    {
        for (const auto &pair : servers)
        {
            destroy_server(pair.first);
        }
        servers.clear();
    }
}

int ServerManager::get_next_port()
{
    int port;
    std::ifstream infile("available_port.txt");
    if (infile.is_open())
    {
        infile >> port;
        infile.close();
    }
    else
    {
        throw std::runtime_error("Configuration file not found");
    }

    port += 1;

    std::ofstream outfile("available_port.txt", std::ios::trunc);
    if (!outfile.is_open())
    {
        throw std::runtime_error("Unable to open configuration file for writing");
    }
    outfile << port;
    outfile.close();

    return port;
}

void ServerManager::display_log(std::string server_name)
{
    auto server_data_it = servers.find(server_name);
    server_data_it->second.controller->read_log();
    return;
}