#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

using namespace websocketpp;

typedef websocketpp::client<websocketpp::config::asio_client> WebSocketClient;

int main(int argc, char* argv[]) {
    std::string uri = "ws://localhost:6969";
    WebSocketClient client;
//    client.set_access_channels(websocketpp::log::alevel::none);
//    client.clear_access_channels(websocketpp::log::alevel::all);
//    client.set_error_channels(websocketpp::log::alevel::none);
//    client.clear_error_channels(websocketpp::log::alevel::all);
    client.set_open_handler([&client](connection_hdl connection){
        client.send(connection, "RunTestSuite:This is not registered", websocketpp::frame::opcode::text);
        client.send(connection, "RunTestSuite:My Cool Test Suite", websocketpp::frame::opcode::text);
    });
    client.init_asio();
    websocketpp::lib::error_code errorCode;
    auto connection = client.get_connection(uri, errorCode);
    client.connect(connection);
    client.run();
}
