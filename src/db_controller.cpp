#include <iostream>
#include <pqxx/pqxx>

#include <db_controller.hpp>

DBController::DBController(std::string dbname,
                           std::string user,
                           std::string password,
                           std::string host_address,
                           std::string port)
    : dbname(dbname), user(user), password(password), host_address(host_address), port(port) {}

pqxx::connection DBController::get_connection()
{
    std::string connection_str = "dbname= " + dbname + " user=" + user + " password=" + password + " hostaddr=" + host_address + " port=" + port;
    pqxx::connection C(connection_str);
    if (!C.is_open())
    {
        std::cerr << "Failed to open database connection" << std::endl;
    };
    return C;
}

pqxx::result DBController::execute_query(std::string query)
{
    pqxx::connection conn = get_connection();
    pqxx::work txn(conn);
    pqxx::result result = txn.exec(query);
    txn.commit();
    conn.close();
    return result;
}
