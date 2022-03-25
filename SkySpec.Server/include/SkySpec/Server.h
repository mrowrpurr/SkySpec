#pragma once

#include <atomic>
#include <chrono>
#include <functional>
#include <utility>

#pragma warning(push)
#include <RE/Skyrim.h>
#include <RE/C/ConsoleLog.h>
#include <REL/Relocation.h>
#include <SKSE/SKSE.h>

#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/common/connection_hdl.hpp>

#include <SkySpec/PapyrusSpec.h>

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

    enum TestRunType { SKSE, Papyrus };

    // Server which orchestrates running test suites (by name)
    class SpecServer {

        WebSocketServer _webSocketServer;

        // Get unique identifier for each test run
        std::atomic<int> _testRun_NextId;

        int _testRun_CurrentId = 0;

        // Map test run ID to a Test Suite Name
        std::unordered_map<int, std::string> _testRun_TestSuiteOrPapyrusScript;

        // Map test run ID to a Connection
        std::unordered_map<int, connection_hdl> _testRun_Connections;

        // Map test run ID to a test type
        std::unordered_map<int, TestRunType> _testRun_Types;

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
            _testRun_TestSuiteOrPapyrusScript.insert_or_assign(testSuiteRunID, testSuiteName);
            _testRun_Connections.insert_or_assign(testSuiteRunID, connection);
            _testRun_ToRunQueue.push(testSuiteRunID);
            _testRun_Types.insert_or_assign(testSuiteRunID, TestRunType::SKSE);
        }

        void RunPapyrusTest(const std::string& scriptName, connection_hdl connection) {
            RE::ConsoleLog::GetSingleton()->Print(std::format("Request to run PAPYRUS test script {}", scriptName).c_str());
            int testSuiteRunID = _testRun_NextId++;
            // TODO - lock :)
            _testRun_TestSuiteOrPapyrusScript.insert_or_assign(testSuiteRunID, scriptName);
            _testRun_Connections.insert_or_assign(testSuiteRunID, connection);
            _testRun_ToRunQueue.push(testSuiteRunID);
            _testRun_Types.insert_or_assign(testSuiteRunID, TestRunType::Papyrus);
        }

        std::queue<int>& GetTestRunQueue() { return _testRun_ToRunQueue; }
        std::string GetTestSuiteOrPapyrusScriptForRunId(int testRunId) { return _testRun_TestSuiteOrPapyrusScript[testRunId]; }
        TestRunType GetTypeOfRunId(int testRunId) { return _testRun_Types[testRunId]; }
        void SetCurrentTestRunId(int testRunId) { _testRun_CurrentId = testRunId; }

        void NotifyText(const std::string& text) {
            RE::ConsoleLog::GetSingleton()->Print(std::format("Trying to send message to notify text {}", text).c_str());
            _webSocketServer.send(_testRun_Connections[_testRun_CurrentId], std::format("NotifyText:{}", text), frame::opcode::text);
            RE::ConsoleLog::GetSingleton()->Print("Did the message send?");
        }
        void NotifyTestPassed(const std::string& text) {
            _webSocketServer.send(_testRun_Connections[_testRun_CurrentId], std::format("NotifyTestPassed:{}", text), frame::opcode::text);
        }
        void NotifyTestFailed(const std::string& text) {
            _webSocketServer.send(_testRun_Connections[_testRun_CurrentId], std::format("NotifyTestFailed:{}", text), frame::opcode::text);
        }

        // TODO Move the Watch functions into another class for better encapsulation please :)
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
                    auto type = server.GetTypeOfRunId(testRunId);
                    if (type == TestRunType::SKSE) {
                        auto testSuiteName = server.GetTestSuiteOrPapyrusScriptForRunId(testRunId);
                        auto& testRegistrations = TestSuiteRegistrations::GetSingleton();
                        if (testRegistrations.IsTestSuiteRegistered(testSuiteName)) {
                            auto fn = testRegistrations.GetTestSuiteRunnerFunction(testSuiteName);
                            RE::ConsoleLog::GetSingleton()->Print(std::format("Running Test Suite '{}'", testSuiteName).c_str());
                            server.SetCurrentTestRunId(testRunId);
                            fn();
                            server.SetCurrentTestRunId(0);
                        } else {
                            queue.push(testRunId);
                        }
                    } else if (type == TestRunType::Papyrus) {
                        auto scriptName = server.GetTestSuiteOrPapyrusScriptForRunId(testRunId);
                        // TODO move the lambdas to functions plz :)
                        PapyrusSpec(scriptName).RunTests(
                                [](const std::string& testName){ SpecServer::GetSingleton().NotifyTestPassed(testName); },
                                [](const std::string& testName){ SpecServer::GetSingleton().NotifyTestFailed(testName); },
                                [](const std::string& message){ SpecServer::GetSingleton().NotifyText(message); }
                        );
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
                RE::ConsoleLog::GetSingleton()->Print(std::format("WEBSOCKET MSG: '{}'\n", messageText).c_str());
                if (messageText == "Quit") {
                    ExitProcess(0);
                } else if (messageText.find("RunTestSuite:") == 0 && messageText.length() > 13) {
                    auto testSuiteName = messageText.substr(13); // length of 'RunTestSuite:'
                    GetSingleton().RunTestSuite(testSuiteName, std::move(connection));
                } else if (messageText.find("RunPapyrusTest:") == 0 && messageText.length() > 13) {
                    auto papyrusScriptName = messageText.substr(15); // length of 'RunPapyrusTest:'
                    GetSingleton().RunPapyrusTest(papyrusScriptName, std::move(connection));
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

    extern "C" __declspec(dllexport) void RegisterTestSuite(const std::string& testSuiteName, std::function<void()> testSuiteRunnerFn);
    extern "C" __declspec(dllexport) void NotifyText(const std::string& text);
    extern "C" __declspec(dllexport) void NotifyTestPassed(const std::string& testName);
    extern "C" __declspec(dllexport) void NotifyTestFailed(const std::string& testName);
}