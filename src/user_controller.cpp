#include <iostream>
#include <pqxx/pqxx>
#include "user_controller.hpp"

std::vector<User>
UserController::get_all_users()
{
    std::vector<User> users;
    pqxx::connection conn = db_controller.get_connection();

    try
    {
        pqxx::work txn(conn);
        pqxx::result result = txn.exec("SELECT username FROM custom_user");

        for (auto row : result)
        {
            std::string username = row["username"].as<std::string>();
            users.emplace_back(username);
        }
        txn.commit();
    }
    catch (std::exception &e)
    {
        std::cerr << "Error fetching all users: " << e.what() << std::endl;
    }
    conn.close();
    return users;
}

std::unique_ptr<User> UserController::create_user(std::string username, std::string password)
{
    pqxx::connection conn = db_controller.get_connection();
    std::string query = "INSERT INTO custom_user (username, password) VALUES ('" + username + "', '" + password + "')";
    db_controller.execute_query(query);
    std::unique_ptr<User> user = std::make_unique<User>(username);
    return user;
}

bool UserController::validate_user(std::string username, std::string entered_password)
{
    pqxx::connection conn = db_controller.get_connection();
    std::string query = "SELECT * FROM custom_user where username = '" + username + "';";
    pqxx::result result = db_controller.execute_query(query);
    conn.close();

    if (result.size() == 0)
    {
        return false;
    };

    std::string password = result[0]["password"].as<std::string>();
    if (entered_password != password)
    {
        return false;
    }
    return true;
}