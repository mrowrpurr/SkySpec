#pragma once

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

    public:
        explicit PapyrusSpec(std::string  scriptName) : _scriptName(std::move(scriptName)) {}

        std::string GetScriptName() { return _scriptName; }

        void RunTestFunction(const std::string testFunctionName) {

        }

        void RunTests(const std::function<void(const std::string&)>& onTestPassed, const std::function<void(const std::string&)>& onTestFailed, const std::function<void(const std::string&)>& onMessage) {
            auto script = PapyrusScript(_scriptName);
            auto functionNames = script.GetFunctionNames();
            onMessage(std::format("{} has {} functions", _scriptName, functionNames.size()));
            for (auto functionName : functionNames) {
                onMessage(std::format("{} has function {}", _scriptName, functionName));
            }
        }
    };
}
