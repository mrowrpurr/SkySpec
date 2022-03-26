#pragma once

#include <string>
#include <utility>
#include <vector>

#pragma warning(push)
#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <RE/C/ConsoleLog.h>
#include <RE/T/TESDataHandler.h>
#include <RE/P/PlayerCharacter.h>
#pragma warning(pop)

using namespace RE::BSScript;
using namespace RE::BSScript::Internal;

namespace Papyrus::Reflection {

    class DispatchFunctionCallback : public RE::BSScript::IStackCallbackFunctor {
    public:
        DispatchFunctionCallback() = default;
        void operator()(RE::BSScript::Variable a_result) override {}
        void SetObject(const RE::BSTSmartPointer<RE::BSScript::Object>&) override {}
    };

    /*
     * ...
     */
    class PapyrusScript {
        bool _initialized = false;
        RE::VMHandle _handle = 0;
        std::string _scriptName;
        std::vector<std::string> _fieldNames;
        std::vector<std::string> _functionNames;
        std::vector<std::string> _propertyNames;

        void _initializeScript() {
            if (_initialized) return;
            else _initialized = true;
            RE::ConsoleLog::GetSingleton()->Print(std::format("Initializing script {}", _scriptName).c_str());

            auto scriptName = _scriptName;
            auto* vm = VirtualMachine::GetSingleton();
            auto* character = RE::PlayerCharacter::GetSingleton();
            auto* handlePolicy = vm->GetObjectHandlePolicy();

            // Get a handle for the character
            _handle = handlePolicy->GetHandleForObject(character->GetFormType(), character);

            // Force load the BindMe script
            vm->linker.Process(RE::BSFixedString(scriptName));

            // Create an object for this script
            RE::BSTSmartPointer<Object> objectPtr;
            vm->CreateObject(scriptName, objectPtr);

            // Bind it!
            auto* bindPolicy = vm->GetObjectBindPolicy();
            bindPolicy->BindObject(objectPtr, _handle);
        }

    public:
        explicit PapyrusScript(std::string scriptName) : _scriptName(std::move(scriptName)) {}

        void RunFunction(const std::string& functionName) {
            RE::ConsoleLog::GetSingleton()->Print(std::format("RunFunction {}()", functionName).c_str());
            if (!_initialized) _initializeScript();

            auto* args = RE::MakeFunctionArguments();
            auto callback = new DispatchFunctionCallback();
            auto callbackPtr = RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>(callback);

            auto* vm = VirtualMachine::GetSingleton();
            vm->DispatchMethodCall(_handle, _scriptName, functionName, args, callbackPtr);
        }

        std::vector<std::string> GetFunctionNames() {
            if (!_initialized) _initializeScript();

            auto* console = RE::ConsoleLog::GetSingleton();
            std::vector<std::string> functionNames;
            auto* vm = VirtualMachine::GetSingleton();
            auto* character = RE::PlayerCharacter::GetSingleton();
            auto* handlePolicy = vm->GetObjectHandlePolicy();
            RE::VMHandle handle = handlePolicy->GetHandleForObject(character->GetFormType(), character);
            for (auto& script : vm->attachedScripts.find(handle)->second) {
                auto* typeInfo = script->GetTypeInfo();
                if (RE::BSFixedString(typeInfo->GetName()) == RE::BSFixedString(_scriptName)) {
                    auto functionCount = typeInfo->GetNumMemberFuncs();
                    auto* functions = typeInfo->GetMemberFuncIter();
                    for (int i = 0; i < functionCount; i++) {
                        auto function = functions[i].func;
                        functionNames.emplace_back(std::format("{}", function->GetName().c_str()));
                    }
                }
            }
            return functionNames;
        }
    };
}
