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
        PapyrusScript _script;
        std::string _scriptName;
        bool _stopRunningTestsAfterAnyFailure = false;
        std::vector<std::string> _testSetupFixtureFunctions;
        std::vector<std::string> _testSetupTestFunctions;
        std::vector<std::string> _testTeardownFixtureFunctions;
        std::vector<std::string> _testTeardownTestFunctions;
        std::vector<std::string> _testFunctions;

        static std::string toLowerCase(const std::string& originalText) {
            std::string text = originalText;
            std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c){ return std::tolower(c); });
            return text;
        }

    public:
        explicit PapyrusSpec(const std::string& scriptName) : _scriptName(scriptName), _script(PapyrusScript(scriptName)) {}

        PapyrusScript& GetScript() { return _script; }
        std::string GetScriptName() { return _scriptName; }

        void RunTestFunction(const std::string testFunctionName) {
            _script.RunFunction(testFunctionName);
        }

        bool IsSetupFixtureFunction(const std::string& functionName) {
            auto lowerFunction = toLowerCase(functionName);
            return lowerFunction == "setupall" || lowerFunction == "beforeall";
        }
        bool IsTeardownFixtureFunction(const std::string& functionName) {
            auto lowerFunction = toLowerCase(functionName);
            return lowerFunction == "teardownall" || lowerFunction == "afterall";
        }
        bool IsSetupFunction(const std::string& functionName) {
            auto lowerFunction = toLowerCase(functionName);
            return lowerFunction == "setup" || lowerFunction == "before";
        }
        bool IsTeardownFunction(const std::string& functionName) {
            auto lowerFunction = toLowerCase(functionName);
            return lowerFunction == "teardown" || lowerFunction == "after";
        }
        bool IsTestFunction(const std::string& functionName) {
            auto lowerFunction = toLowerCase(functionName);
            return lowerFunction.starts_with("it_") || lowerFunction.starts_with("test") || lowerFunction.ends_with("test");
        }

        void DetectTestFunctions() {
            auto functionNames = GetScript().GetFunctionNames();
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
                } else {
                    RE::ConsoleLog::GetSingleton()->Print(std::format("Unknown function: {}", functionName).c_str());
                }
            }
        }

        void RunFunction(const std::string& functionName) {
            GetScript().RunFunction(functionName);
        }

        void RunFunctions(const std::vector<std::string>& functionNames) {
            for (const auto& functionName : functionNames) { RunFunction(functionName); }
        }

        // TODO - maybe remove these arguments? and don't use the callbacks at all?
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
