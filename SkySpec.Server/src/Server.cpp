#include "SkySpec/Server.h"

void SkySpec::Server::RegisterTestSuite(const std::string& testSuiteName, std::function<void()> testSuiteRunnerFn) {
    SkySpec::Server::TestSuiteRegistrations::GetSingleton().RegisterTestSuiteRunnerFunction(testSuiteName, testSuiteRunnerFn);
}

void SkySpec::Server::NotifyText(const std::string& text) {
    SkySpec::Server::SpecServer::GetSingleton().NotifyText(text);
}

void SkySpec::Server::NotifyTestPassed(const std::string& testName) {
    SkySpec::Server::SpecServer::GetSingleton().NotifyTestPassed(testName);
}

void SkySpec::Server::NotifyTestFailed(const std::string& testName) {
    SkySpec::Server::SpecServer::GetSingleton().NotifyTestFailed(testName);
}
