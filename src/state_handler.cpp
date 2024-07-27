#include <pqxx/pqxx>
#include <iostream>
#include <string>

#include "state_handler.hpp"
#include "states.hpp"
#include <thread>

State StateHandler::auth_page()
{
    std::cout << "Welcome to my C++ App\n"
                 "1) Login\n"
                 "2) Register\n"
                 "3) Quit\n"
              << std::endl;

    std::string raw_choice;
    std::getline(std::cin, raw_choice);

    int choice;
    try
    {
        choice = std::stoi(raw_choice);
    }
    catch (std::invalid_argument)
    {
        std::cout << "Input a number from 1 to 3!!" << std::endl;
        return State::AUTH;
    }

    switch (choice)
    {
    case 1:
        return State::LOGIN;
    case 2:
        return State::REGISTER;
    case 3:
        return State::QUIT;
    default:
        std::cout << "Input a number from 1 to 3!!" << std::endl;
        return State::AUTH;
    }

    return State::LOGIN;
}

State StateHandler::login_page()
{
    std::string username;
    std::string password;

    std::cout << "Enter your username (q to quit): ";
    std::getline(std::cin, username);

    if (username == "q")
    {
        return State::AUTH;
    }

    std::cout << "Enter your password: ";
    std::getline(std::cin, password);

    bool is_password_valid = user_controller.validate_user(username, password);
    if (!is_password_valid)
    {
        std::cout << "Incorrect username or password\n"
                  << std::endl;
        return State::LOGIN;
    }
    std::string user_id = user_controller.get_user_id(username);
    user = User(username, user_id);

    return State::MENU;
}

State StateHandler::register_page()
{
    std::string username;
    std::string password;
    std::string password2;

    std::cout << "Enter your username: ";
    std::getline(std::cin, username);

    std::cout << "Enter your password: ";
    std::getline(std::cin, password);

    std::cout << "Re-enter your password: ";
    std::getline(std::cin, password2);

    if (password != password2)
    {
        std::cout << "The passwords don't match!\n"
                  << std::endl;
        return State::REGISTER;
    }
    try
    {
        user = user_controller.create_user(username, password);
    }
    catch (const pqxx::unique_violation &e)
    {
        std::cout << "An user with this username already exist!\n"
                  << std::endl;
        return State::REGISTER;
    }
    return State::MENU;
}

State StateHandler::menu_page()
{
    std::cout << "\n1) Join a server\n"
                 "2) Create a server\n"
                 "3) View servers\n"
                 "4) Quit\n"
              << std::endl;

    std::string raw_choice;
    std::getline(std::cin, raw_choice);

    int choice;
    try
    {
        choice = std::stoi(raw_choice);
    }
    catch (std::invalid_argument)
    {
        std::cout << "Input a number from 1 to 4!!" << std::endl;
        return State::MENU;
    }

    switch (choice)
    {
    case 1:
        return State::JOIN_SERVER;
    case 2:
        return State::CREATE_SERVER;
    case 3:
        return State::VIEW_SERVERS;
    case 4:
        return State::QUIT;
    default:
        std::cout << "Input a number from 1 to 4!!" << std::endl;
        return State::MENU;
    }
}

State StateHandler::join_server_page()
{
    std::string input;

    std::map<std::string, int> servers = server_manager.get_all_servers();

    if (servers.size() == 0)
    {
        std::cout << "No servers currently available to join."
                  << std::endl;
        return State::MENU;
    }

    std::cout << "Here is a list of servers to join (name: port).\n Please type in the server name you want to join (q to quit)." << std::endl;
    for (const auto &server : servers)
    {
        std::cout << server.first << ": " << server.second << std::endl;
    };
    std::getline(std::cin, input);

    bool invalid = true;
    int port;
    while (invalid)
    {
        if (input == "q")
        {
            return State::MENU;
        }

        auto it = servers.find(input);
        if (it == servers.end())
        {
            std::cout << "Pleasa type in a server that exists!" << std::endl;
            std::getline(std::cin, input);
        }
        else
        {
            port = it->second;
            invalid = false;
        }
    }
    client_controller.join_server(user->get_username(), user->get_id(), port);
    return State::MENU;
}

State StateHandler::create_server_page()
{
    std::cout << "Enter the server name (q to quit): ";
    std::string server_name;
    std::getline(std::cin, server_name);

    if (server_name == "q")
    {
        return State::MENU;
    }

    server_manager.create_server(server_name, user->get_username());

    return State::MENU;
}

State StateHandler::view_servers_page()
{
    std::string input;
    std::map<std::string, int> servers = server_manager.get_servers();

    if (servers.size() == 0)
    {
        std::cout << "You have not created any servers."
                  << std::endl;
        return State::MENU;
    }

    std::cout << "Here are all of your servers, type in the server you want to inspect." << std::endl;
    for (const auto &server : servers)
    {
        std::cout << server.first << ": " << server.second << std::endl;
    }
    std::getline(std::cin, input);
    bool invalid = true;
    while (invalid)
    {
        if (input == "q")
        {
            return State::MENU;
        }
        auto it = servers.find(input);
        if (it == servers.end())
        {
            std::cout << "No server found with that name." << std::endl;
            std::getline(std::cin, input);
        }
        else
        {
            invalid = false;
        }
    }
    manage_server_page(input);
    return State::MENU;
}

void StateHandler::manage_server_page(std::string server_name)
{
    std::cout << "1) View logs\n"
                 "2) View users\n"
                 "3) Shut down server\n"
                 "4) Back\n"
              << std::endl;

    std::string raw_choie;
    int choice;
    std::getline(std::cin, raw_choie);
    bool invalid = true;
    while (invalid)
    {
        try
        {
            choice = std::stoi(raw_choie);
            if (choice > 0 && choice < 5)
            {
                invalid = false;
            }
            else
            {
                throw std::invalid_argument("");
            }
        }
        catch (std::invalid_argument)
        {
            std::cout << "Enter a valid choice!" << std::endl;
            std::getline(std::cin, raw_choie);
        }
    }
    switch (choice)
    {
    case 1:
        std::cout << "here are the logs" << std::endl;
        server_log_page(server_name);
        return;
    case 2:
        std::cout << "here are the users" << std::endl;
        server_manager.get_users_in_server(server_name);
        return;
    case 3:
        server_manager.close(server_name);
        return;
    case 4:
        return;
    }
}

void StateHandler::server_log_page(std::string server_name)
{
    server_manager.display_log(server_name);
}

void StateHandler::quit()
{
    if (user)
    {
        server_manager.close();
        user_controller.unset_as_active(user->get_username());
    }
    client_controller.cleanup();
}