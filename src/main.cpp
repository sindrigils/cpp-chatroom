#include <iostream>
#include <map>
#include <set>

#include "state_handler.hpp"

int main()
{
    StateHandler state_handle{};
    std::unordered_map<State, std::function<State()>> state_lookup = {
        {State::AUTH, std::bind(&StateHandler::auth_page, &state_handle)},
        {State::LOGIN, std::bind(&StateHandler::login_page, &state_handle)},
        {State::REGISTER, std::bind(&StateHandler::register_page, &state_handle)},
        {State::MENU, std::bind(&StateHandler::menu_page, &state_handle)},
        {State::CREATE_SERVER, std::bind(&StateHandler::create_server_page, &state_handle)},
        {State::JOIN_SERVER, std::bind(&StateHandler::join_server_page, &state_handle)},
    };

    State state = State::AUTH;
    while (state != State::QUIT)
    {
        state = state_lookup[state]();
    }
    state_handle.quit();
    std::cout << "Bye bye" << std::endl;
    return 0;
}
