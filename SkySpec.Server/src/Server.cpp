#include "SkySpec/Server.h"

void SkySpec::Server::RegisterTestSuite(const std::string& testSuiteName, std::function<void()> testSuiteRunnerFn) {
    SkySpec::Server::TestSuiteRegistrations::GetSingleton().RegisterTestSuiteRunnerFunction(testSuiteName, testSuiteRunnerFn);
}

void SkySpec::Server::NotifyText(int testRunId, const std::string& testSuiteName, const std::string& text) {

}

void SkySpec::Server::NotifyTestPassed(int testRunId, const std::string& testSuiteName, const std::string& testName) {

}

void SkySpec::Server::NotifyTestFailed(int testRunId, const std::string& testSuiteName, const std::string& testName) {

}
