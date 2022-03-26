#pragma once

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
        WebSocketClient _webSocketClient;
        int _webSocketPort = 6969;
        std::string _skyrimRunCommand;
        std::string _startingCell;
        std::vector<std::string> _testSuiteNames;
        bool _runGameOnStart = true;
        bool _quitGameOnFinish = true;
        connection_hdl _connection;
        bool _connected = false;
        std::queue<std::string> _messagesToSendOnConnected;

        Runner() = default;
    public:
        Runner(const Runner&) = delete;
        Runner &operator=(const Runner&) = delete;
        static Runner& GetSingleton() {
            static Runner runner;
            return runner;
        }

        void Configure(const RunnerConfig& config) {
            _webSocketPort = config.WebSocketPort;
            _skyrimRunCommand = config.SkyrimRunCommand;
            _startingCell = config.StartingCell;
            _testSuiteNames = config.TestSuites;
            _runGameOnStart = config.RunGameOnStart;
            _quitGameOnFinish = config.QuitGameOnFinish;
        }

        void SetConnection(const connection_hdl& connection) {
            _connected = true;
            _connection = connection;
            SendQueuedMessages();
        }

        void SendQueuedMessages() {
            if (_connected) {
                while (! _messagesToSendOnConnected.empty()) {
                    SendTextMessage(_messagesToSendOnConnected.front());
                    std::cout << std::format("Sending queued message: {}\n", _messagesToSendOnConnected.front());
                    _messagesToSendOnConnected.pop();
                }
            }
        }

        void SendTextMessage(const std::string& messageText) {
            if (_connected) {
                std::cout << std::format("Send Message Via Websocket: '{}'\n", messageText);
                _webSocketClient.send(_connection, messageText, frame::opcode::text);
            } else {
                _messagesToSendOnConnected.push(messageText);
            }
        }

        void RunPapyrusTestScript(const std::string& scriptName) {
            SendTextMessage(std::format("RunPapyrusTest:{}", scriptName));
        }

        void RunSkseTestSuite(const std::string& testSuiteName) {
            SendTextMessage(std::format("RunTestSuite:{}", testSuiteName));
        }

        void Run() {
            auto uri = std::format("ws://localhost:{}", _webSocketPort);
//            _webSocketClient.set_access_channels(websocketpp::log::alevel::none);
//            _webSocketClient.clear_access_channels(websocketpp::log::alevel::all);
//            _webSocketClient.set_error_channels(websocketpp::log::alevel::none);
//            _webSocketClient.clear_error_channels(websocketpp::log::alevel::all);
            _webSocketClient.set_open_handler([](const connection_hdl& connection){
                Runner::GetSingleton().SetConnection(connection);
            });
            _webSocketClient.set_message_handler([](const connection_hdl&, const WebSocketMessagePtr& message){
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
            _webSocketClient.init_asio();
            websocketpp::lib::error_code errorCode;
            auto connection = _webSocketClient.get_connection(uri, errorCode);
            _webSocketClient.connect(connection);
            _webSocketClient.run();
        }

        std::thread RunBackground() { std::thread t([](){ Runner::GetSingleton().Run(); }); t.detach(); return t; }
    };

    void Run(const RunnerConfig& config) {
        auto& runner = Runner::GetSingleton();
        runner.Configure(config);
        runner.Run();
    }

    void Run(int, char* []) {
        // TODO - turn arguments into test suite names but also add flags for the rest of things, e.g. coc etc
        Runner::GetSingleton().Run(); // Just use the defaults for testing for right now
    }
}
