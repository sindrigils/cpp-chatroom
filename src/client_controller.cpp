#include "client_controller.hpp"
#include <iostream>
#include <thread>
#include <chrono>

ClientController::ClientController() : running(true), message_win(nullptr), input_win(nullptr) {}

ClientController::~ClientController()
{
    cleanup();
}

void ClientController::set_up()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    int term_height, term_width;
    getmaxyx(stdscr, term_height, term_width);

    int message_height = term_height * 0.8;
    int input_height = term_height * 0.2;
    int width = term_width;

    message_win = newwin(message_height, width, 0, 0);
    input_win = newwin(input_height, width, message_height, 0);

    box(message_win, 0, 0);
    box(input_win, 0, 0);

    refresh();
    wrefresh(message_win);
    wrefresh(input_win);
}

void ClientController::join_server(int port)
{
    set_up();
    websocket_client.init_asio();
    websocket_client.clear_access_channels(websocketpp::log::alevel::all);
    websocket_client.clear_error_channels(websocketpp::log::elevel::all);
    websocket_client.set_access_channels(websocketpp::log::alevel::app);
    websocket_client.set_error_channels(websocketpp::log::elevel::warn | websocketpp::log::elevel::rerror);

    websocket_client.set_open_handler([&](websocketpp::connection_hdl hdl)
                                      {
        websocketpp::lib::error_code ec;
        if (ec) {
            std::cout << "Error sending initial message: " << ec.message() << std::endl;
            websocket_client.close(hdl, websocketpp::close::status::going_away, "Error sending initial message");
        } });

    websocket_client.set_message_handler([&](websocketpp::connection_hdl hdl, websocketpp::config::asio_client::message_type::ptr msg)
                                         {
        std::lock_guard<std::mutex> lock(messages_mutex);
        messages.push_back(msg->get_payload()); });

    websocketpp::lib::error_code ec;
    auto con = websocket_client.get_connection("ws://localhost:" + std::to_string(port), ec);
    if (ec)
    {
        std::cout << "Error initiating connection: " << ec.message() << std::endl;
        return;
    }

    websocket_client.connect(con);

    std::thread display_thread(&ClientController::display_messages, this);
    std::thread input_thread(&ClientController::handle_input, this, con);

    websocket_client.run();

    running = false;
    display_thread.join();
    input_thread.join();
    cleanup();
}

void ClientController::on_message(websocketpp::connection_hdl hdl, websocketpp::config::asio_client::message_type::ptr msg)
{
    std::lock_guard<std::mutex> lock(messages_mutex);
    messages.push_back(msg->get_payload());
}

void ClientController::display_messages()
{
    int max_lines = getmaxy(message_win) - 2;
    int start_line = 0;

    while (running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        werase(message_win);
        box(message_win, 0, 0);

        std::lock_guard<std::mutex> lock(messages_mutex);
        int y = 1;
        int num_messages = messages.size();

        if (num_messages > max_lines)
        {
            start_line = num_messages - max_lines;
        }

        for (int i = start_line; i < num_messages; ++i)
        {
            mvwprintw(message_win, y++, 1, "%s", messages[i].c_str());
        }
        wrefresh(message_win);
    }
}

void ClientController::handle_input(websocketpp::connection_hdl con)
{
    char input[80];
    int input_len = 0;
    int max_input_len = 78;

    while (running)
    {
        werase(input_win);
        box(input_win, 0, 0);
        mvwprintw(input_win, 1, 1, "> %s", input);

        int ch = wgetch(input_win);

        if (ch == '\n')
        {
            std::lock_guard<std::mutex> lock(messages_mutex);

            if (std::string(input) == "quit")
            {
                running = false;
                break;
            }

            messages.push_back("You: " + std::string(input));
            wclear(input_win);
            box(input_win, 0, 0);
            wrefresh(input_win);

            websocketpp::lib::error_code ec;
            websocket_client.send(con, std::string(input), websocketpp::frame::opcode::text, ec);
            if (ec)
            {
                std::cout << "Error sending message: " << ec.message() << std::endl;
            }

            input[0] = '\0';
            input_len = 0;
        }
        else if (ch == KEY_BACKSPACE || ch == 127)
        {
            if (input_len > 0)
            {
                input[--input_len] = '\0';
            }
        }
        else if (input_len < max_input_len && ch >= 32 && ch <= 126)
        {
            input[input_len++] = ch;
            input[input_len] = '\0';
        }
    }
    websocket_client.close(con, websocketpp::close::status::going_away, "User quit");
}

void ClientController::cleanup()
{
    endwin();
    delwin(message_win);
    delwin(input_win);
}
