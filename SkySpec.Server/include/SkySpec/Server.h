#pragma once

#pragma warning(push)
#include <RE/Skyrim.h>
#include <REL/Relocation.h>
#include <SKSE/SKSE.h>

#include <atomic>
#include <chrono>
#include <functional>
#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/common/connection_hdl.hpp>
#include <utility>
#include <RE/C/ConsoleLog.h>

using namespace websocketpp;
using namespace std::chrono_literals;

typedef websocketpp::server<config::asio> WebSocketServer;
typedef WebSocketServer::message_ptr WebSocketMessagePtr;
typedef WebSocketServer::message_handler WebSocketMessageHandler;

namespace SkySpec::Server {

    // Store simple function callbacks for invoking test suites
    class TestSuiteRegistrations {
        std::unordered_map<std::string, std::function<void()>> _testSuiteRunnerFunctions;
        TestSuiteRegistrations() = default;
    public:
        TestSuiteRegistrations(const TestSuiteRegistrations&) = delete;
        TestSuiteRegistrations &operator=(const TestSuiteRegistrations&) = delete;
        static TestSuiteRegistrations& GetSingleton() {
            static TestSuiteRegistrations testSuiteRegistrations;
            return testSuiteRegistrations;
        }
        void RegisterTestSuiteRunnerFunction(const std::string& testSuiteName, std::function<void()> testSuiteRunnerFn) {
            RE::ConsoleLog::GetSingleton()->Print(std::format("Loaded Test Suite '{}'", testSuiteName).c_str());
            _testSuiteRunnerFunctions.insert_or_assign(testSuiteName, testSuiteRunnerFn);
        }
        bool IsTestSuiteRegistered(const std::string& testSuiteName) { return _testSuiteRunnerFunctions.contains(testSuiteName); }
        std::function<void()> GetTestSuiteRunnerFunction(const std::string& testSuiteName) { return _testSuiteRunnerFunctions[testSuiteName]; }
    };

    // Server which orchestrates running test suites (by name)
    class SpecServer {
        WebSocketServer _webSocketServer;

        // Get unique identifier for each test run
        std::atomic<int> _testRun_NextId;

        // Map test run ID to a Test Suite Name
        std::unordered_map<int, std::string> _testRun_TestSuiteNames;

        // Map test run ID to a Connection
        std::unordered_map<int, connection_hdl> _testRun_Connections;

        // The queue of test runs to run
        std::queue<int> _testRun_ToRunQueue;

        SpecServer() = default;

    public:
        SpecServer(const SpecServer&) = delete;
        SpecServer &operator=(const SpecServer&) = delete;
        static SpecServer& GetSingleton() {
            static SpecServer server;
            return server;
        }

        // Called by connection to request a test suite be run by name
        // Creates a 'testRun'
        void RunTestSuite(const std::string& testSuiteName, connection_hdl connection) {
            RE::ConsoleLog::GetSingleton()->Print(std::format("Request to run test suite {}", testSuiteName).c_str());
            int testSuiteRunID = _testRun_NextId++;
            // TODO - lock :)
            _testRun_TestSuiteNames.insert_or_assign(testSuiteRunID, testSuiteName);
            _testRun_Connections.insert_or_assign(testSuiteRunID, connection);
            _testRun_ToRunQueue.push(testSuiteRunID);
        }

        std::queue<int>& GetTestRunQueue() { return _testRun_ToRunQueue; }
        std::string GetTestSuiteNameForRunId(int testRunId) { return _testRun_TestSuiteNames[testRunId]; }

        static void WatchQueueForTestsToRun() {
            std::thread testRunWatcherThread(WatchQueueLoop);
            testRunWatcherThread.detach();
        }

        [[noreturn]] static void WatchQueueLoop() {
            while (true) {
                auto& server = GetSingleton();
                auto& queue = server.GetTestRunQueue();
                if (!queue.empty()) {
                    auto testRunId = queue.front(); queue.pop(); // TODO - lock!
                    auto testSuiteName = server.GetTestSuiteNameForRunId(testRunId);
                    auto& testRegistrations = TestSuiteRegistrations::GetSingleton();
                    if (testRegistrations.IsTestSuiteRegistered(testSuiteName)) {
                        auto fn = testRegistrations.GetTestSuiteRunnerFunction(testSuiteName);
                        RE::ConsoleLog::GetSingleton()->Print(std::format("Running Test Suite '{}'", testSuiteName).c_str());
                        fn();
                    } else {
                        queue.push(testRunId);
                    }
                }
                std::this_thread::sleep_for(1s);
            }
        }

        void Run() {
            std::thread serverThread([](){ SpecServer::GetSingleton().RunServer(); });
            serverThread.detach();
        }

        void RunServer() {
            _webSocketServer.set_message_handler([](connection_hdl connection, WebSocketMessagePtr message){
                auto messageText = message->get_payload();
                if (messageText == "Quit") {
                    ExitProcess(0);
                } else if (messageText.find("RunTestSuite:") == 0 && messageText.length() > 13) {
                    auto testSuiteName = messageText.substr(13); // length of 'RunTestSuite:'
                    GetSingleton().RunTestSuite(testSuiteName, std::move(connection));
                }
            });
            try {
                WatchQueueForTestsToRun();
                _webSocketServer.init_asio();
                _webSocketServer.listen(6969);
                RE::ConsoleLog::GetSingleton()->Print("SkySpec Running on 6969");
                _webSocketServer.start_accept();
                _webSocketServer.run();
            } catch (websocketpp::exception const & e) {
                RE::ConsoleLog::GetSingleton()->Print(std::format("[SkySpec] Server Error: {}", e.what()).c_str());
            } catch (...) {
                RE::ConsoleLog::GetSingleton()->Print("[SkySpec] Server Error");
            }
        }
    };

    __declspec(dllexport) void RegisterTestSuite(const std::string& testSuiteName, std::function<void()> testSuiteRunnerFn);
    __declspec(dllexport) void NotifyText(int testRunId, const std::string& testSuiteName, const std::string& text);
    __declspec(dllexport) void NotifyTestPassed(int testRunId, const std::string& testSuiteName, const std::string& testName);
    __declspec(dllexport) void NotifyTestFailed(int testRunId, const std::string& testSuiteName, const std::string& testName);
}