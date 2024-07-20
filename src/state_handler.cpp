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
        std::cout << "The passwords dont match!\n"
                  << std::endl;
        return State::REGISTER;
    }
    try
    {
        std::unique_ptr<User> user = user_controller.create_user(username, password);
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
    std::cout << "1) Join a server\n"
                 "2) Create a server\n"
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
        return State::MENU;
    }

    switch (choice)
    {
    case 1:
        return State::JOIN_SERVER;

    case 2:
        return State::CREATE_SERVER;
    case 3:
        return State::QUIT;
    default:
        std::cout << "Input a number from 1 to 3!!" << std::endl;
        return State::MENU;
    }
}

State StateHandler::join_server_page()
{
    client_controller.join_server();
    // get all servers
    // display all servers
    // get input (number)
    // join

    return State::MENU;
}

State StateHandler::create_server_page()
{
    std::cout << "Enter the server name: ";
    std::string server_name;
    std::getline(std::cin, server_name);
    server_manager.create_server(server_name, "sindri");
    server_manager.destroy_server(server_name);
    // std::thread run_server_thread(&ServerController::run_server, &server_controller);
    // server_controller.shut_down_server(&run_server_thread);

    return State::MENU;
}