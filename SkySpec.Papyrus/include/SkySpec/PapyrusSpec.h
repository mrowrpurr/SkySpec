#pragma once

#include <algorithm>
#include <cctype>
#include <iostream>
#include <functional>
#include <string>
#include <utility>

#pragma warning(push)
#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <RE/C/ConsoleLog.h>
#pragma warning(pop)

#include <Papyrus/Reflection.h>

using namespace Papyrus::Reflection;

namespace SkySpec {
    class PapyrusSpec {
        std::string _scriptName;
        bool _stopRunningTestsAfterAnyFailure = false;
        std::vector<std::string> _testSetupFixtureFunctions;
        std::vector<std::string> _testSetupTestFunctions;
        std::vector<std::string> _testTeardownFixtureFunctions;
        std::vector<std::string> _testTeardownTestFunctions;
        std::vector<std::string> _testFunctions;

        void toLowerCase(std::string text) {
            std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c){ return std::tolower(c); });
        }

    public:
        explicit PapyrusSpec(std::string scriptName) : _scriptName(std::move(scriptName)) {}

        std::string GetScriptName() { return _scriptName; }

        void RunTestFunction(const std::string testFunctionName) {

        }

        bool IsSetupFixtureFunction(const std::string& functionName) {
            return RE::BSFixedString(functionName) == RE::BSFixedString("SetupAll")
                || RE::BSFixedString(functionName) == RE::BSFixedString("BeforeAll");
        }
        bool IsTeardownFixtureFunction(const std::string& functionName) {
            return RE::BSFixedString(functionName) == RE::BSFixedString("TeardownAll")
                || RE::BSFixedString(functionName) == RE::BSFixedString("AfterAll");
        }
        bool IsSetupFunction(const std::string& functionName) {
            return RE::BSFixedString(functionName) == RE::BSFixedString("Setup")
                || RE::BSFixedString(functionName) == RE::BSFixedString("Before");
        }
        bool IsTeardownFunction(const std::string& functionName) {
            return RE::BSFixedString(functionName) == RE::BSFixedString("Teardown")
                || RE::BSFixedString(functionName) == RE::BSFixedString("After");
        }
        bool IsTestFunction(const std::string& functionName) {
            std::string lowerFunction = functionName;
            toLowerCase(lowerFunction);
            return lowerFunction.starts_with("if_") || lowerFunction.starts_with("test") || lowerFunction.ends_with("test");
        }

        void DetectTestFunctions() {
            auto script = PapyrusScript(_scriptName);
            auto functionNames = script.GetFunctionNames();
            for (const auto& functionName : functionNames) {
                if (IsTestFunction(functionName)) {
                    _testFunctions.emplace_back(functionName);
                } else if (IsSetupFunction(functionName)) {
                    _testSetupTestFunctions.emplace_back(functionName);
                } else if (IsTeardownFunction(functionName)) {
                    _testTeardownTestFunctions.emplace_back(functionName);
                } else if (IsSetupFixtureFunction(functionName)) {
                    _testSetupFixtureFunctions.emplace_back(functionName);
                } else if (IsTeardownFixtureFunction(functionName)) {
                    _testTeardownFixtureFunctions.emplace_back(functionName);
                }
            }
        }

        void RunFunction(const std::string& functionName) {

        }

        void RunFunctions(const std::vector<std::string>& functionNames) {

        }

        void RunTests(const std::function<void(const std::string&)>& onTestPassed, const std::function<void(const std::string&)>& onTestFailed, const std::function<void(const std::string&)>& onMessage) {
            DetectTestFunctions();
            if (! _testFunctions.empty()) {
                RunFunctions(_testSetupFixtureFunctions);
                for (const auto& testFunctionName : _testFunctions) {
                    RunFunctions(_testSetupTestFunctions);
                    RunFunction(testFunctionName);
                    RunFunctions(_testTeardownTestFunctions);
                }
                RunFunctions(_testTeardownFixtureFunctions);
            }
        }
    };
}
