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
            std::string id = row["id"].as<std::string>();
            users.emplace_back(username, id);
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

User UserController::create_user(std::string username, std::string password)
{
    pqxx::connection conn = db_controller.get_connection();
    std::string query = "INSERT INTO custom_user (username, password, is_active) VALUES ('" + username + "', '" + password + "', true)";
    db_controller.execute_query(query);
    std::string user_id = db_controller.execute_query("SELECT id from custom_user where username='" + username + "';")[0]["id"].as<std::string>();
    User user(username, user_id);
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
    bool is_active = result[0]["is_active"].as<bool>();
    if (entered_password != password || is_active)
    {
        return false;
    }
    set_as_active(username);
    return true;
}

std::string UserController::get_user_id(std::string username)
{
    std::string query = "SELECT id from custom_user where username ='" + username + "';";
    return db_controller.execute_query(query)[0]["id"].as<std::string>();
}

void UserController::set_as_active(std::string username)
{
    db_controller.execute_query("UPDATE custom_user SET is_active = true where username ='" + username + "';");
}

void UserController::unset_as_active(std::string username)
{
    db_controller.execute_query("UPDATE custom_user SET is_active = false where username ='" + username + "';");
}