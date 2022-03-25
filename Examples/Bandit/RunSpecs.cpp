#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

using namespace websocketpp;

typedef websocketpp::client<websocketpp::config::asio_client> WebSocketClient;
typedef websocketpp::config::asio_client::message_type::ptr WebSocketMessagePtr;

int main(int argc, char* argv[]) {
    std::string uri = "ws://localhost:6969";
    WebSocketClient client;
    client.set_access_channels(websocketpp::log::alevel::none);
    client.clear_access_channels(websocketpp::log::alevel::all);
    client.set_error_channels(websocketpp::log::alevel::none);
    client.clear_error_channels(websocketpp::log::alevel::all);
    client.set_open_handler([&client](const connection_hdl& connection){
        client.send(connection, "RunTestSuite:This is not registered", websocketpp::frame::opcode::text);
        client.send(connection, "RunTestSuite:My Cool Test Suite", websocketpp::frame::opcode::text);
    });
    client.set_message_handler([](const connection_hdl& connection, const WebSocketMessagePtr& message){
        auto messageText = message->get_payload();
        auto separatorIndex = messageText.find(':');
        if (separatorIndex != std::string::npos && messageText.length() > (separatorIndex + 1)) {
            auto command = messageText.substr(0, separatorIndex);
            std::string text = messageText.substr(separatorIndex + 1);
            if (command == "NotifyText") {
                std::cout << text << std::endl;
            } else if (command == "NotifyTestPassed") {
                std::cout << std::format("{} [PASSED]", text) << std::endl;
            } else if (command == "NotifyTestFailed") {
                std::cout << std::format("{} [FAILED]", text) << std::endl;
            }
        }
    });
    client.init_asio();
    websocketpp::lib::error_code errorCode;
    auto connection = client.get_connection(uri, errorCode);
    client.connect(connection);
    client.run();
}
