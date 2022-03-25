#include <iostream>
#include <vector>

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

using namespace websocketpp;

typedef websocketpp::client<websocketpp::config::asio_client> WebSocketClient;
typedef websocketpp::config::asio_client::message_type::ptr WebSocketMessagePtr;

namespace SkySpec {

    struct RunnerConfig {
        int WebSocketPort = 6969;
        std::string SkyrimRunCommand;
        std::string StartingCell;
        std::vector<std::string> TestSuites;
        bool RunGameOnStart = true;
        bool QuitGameOnFinish = true;
    };

    class Runner {
        int _webSocketPort = 6969;
        std::string _skyrimRunCommand;
        std::string _startingCell;
        std::vector<std::string> _testSuiteNames;
        bool _runGameOnStart = true;
        bool _quitGameOnFinish = true;

    public:
        Runner() = default;
        explicit Runner(const RunnerConfig& config) {
            _webSocketPort = config.WebSocketPort;
            _skyrimRunCommand = config.SkyrimRunCommand;
            _startingCell = config.StartingCell;
            _testSuiteNames = config.TestSuites;
            _runGameOnStart = config.RunGameOnStart;
            _quitGameOnFinish = config.QuitGameOnFinish;
        }

        void Run() const {
            auto uri = std::format("ws://localhost:{}", _webSocketPort);
            WebSocketClient client;
            client.set_access_channels(websocketpp::log::alevel::none);
            client.clear_access_channels(websocketpp::log::alevel::all);
            client.set_error_channels(websocketpp::log::alevel::none);
            client.clear_error_channels(websocketpp::log::alevel::all);
            client.set_open_handler([&client](const connection_hdl& connection){
                // TODO either all of the test suites which are configured or RunAll:
                // Start off with RunAll:
                client.send(connection, "RunTestSuite:My Cool Test Suite", websocketpp::frame::opcode::text);
            });
            client.set_message_handler([](const connection_hdl&, const WebSocketMessagePtr& message){
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
    };

    void Run(const RunnerConfig& config) {
        Runner(config).Run();
    }

    void Run(int, char* []) {
        // TODO - turn arguments into test suite names but also add flags for the rest of things, e.g. coc etc
        Runner().Run(); // Just use the defaults for testing for right now
    }
}
