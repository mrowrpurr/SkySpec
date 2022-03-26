#pragma once

#include <string>
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

    /*
     * ...
     */
    class PapyrusScript {
        bool _initialized = false;
        std::string _scriptName;
        std::vector<std::string> _fieldNames;
        std::vector<std::string> _functionNames;
        std::vector<std::string> _propertyNames;

    public:
        PapyrusScript(const std::string& scriptName) : _scriptName(scriptName) {}

        std::vector<std::string> GetFunctionNames() {
            auto* console = RE::ConsoleLog::GetSingleton();
            if (!_initialized) InitializeScript();
            std::vector<std::string> functionNames;
            auto* vm = VirtualMachine::GetSingleton();
            auto* character = RE::PlayerCharacter::GetSingleton();
            auto* handlePolicy = vm->GetObjectHandlePolicy();
            RE::VMHandle handle = handlePolicy->GetHandleForObject(character->GetFormType(), character);
            console->Print("Looking for scripts for handle...");
            for (auto& script : vm->attachedScripts.find(handle)->second) {
                auto* typeInfo = script->GetTypeInfo();
                console->Print(std::format("Attached script type name: {}", typeInfo->GetName()).c_str());
                if (RE::BSFixedString(typeInfo->GetName()) == RE::BSFixedString(_scriptName)) {
                    // We found it!
                    console->Print("FOUND IT");
                    auto functionCount = typeInfo->GetNumMemberFuncs();
                    auto* functions = typeInfo->GetMemberFuncIter();
                    for (int i = 0; i < functionCount; i++) {
                        auto function = functions[i].func;
                        functionNames.emplace_back(std::format("{} ({})", function->GetName().c_str(), function->GetDocString().c_str()));
                    }
                }
            }
            return functionNames;
        }

        // CLEAN UP
        void InitializeScript() {
            if (_initialized) return;
            else _initialized = true;

            auto scriptName = _scriptName;
            auto* vm = VirtualMachine::GetSingleton();
            auto* character = RE::PlayerCharacter::GetSingleton();
            auto* handlePolicy = vm->GetObjectHandlePolicy();

            // Get a handle for the character
            RE::VMHandle handle = handlePolicy->GetHandleForObject(character->GetFormType(), character);

            RE::ConsoleLog::GetSingleton()->Print("Binding the BindMe script...");

            // Force load the BindMe script
            vm->linker.Process(RE::BSFixedString(scriptName));

            // Create an object for this script
            RE::BSTSmartPointer<Object> objectPtr;
            vm->CreateObject(scriptName, objectPtr);

            // Bind it!
            auto* bindPolicy = vm->GetObjectBindPolicy();
            bindPolicy->BindObject(objectPtr, handle);
        }
    };
}
