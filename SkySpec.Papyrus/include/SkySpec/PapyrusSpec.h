#include <functional>
#include <string>
#include <utility>

#pragma warning(push)

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <RE/C/ConsoleLog.h>
#include <RE/T/TESDataHandler.h>
#include <RE/P/PlayerCharacter.h>

//using namespace RE;
using namespace RE::BSScript;
using namespace RE::BSScript::Internal;

namespace SkySpec {
    class PapyrusSpec {
        std::string _scriptName;

    public:
        explicit PapyrusSpec(std::string  scriptName) : _scriptName(std::move(scriptName)) {}

        std::string GetScriptName() { return _scriptName; }

        void InitializeScript() {
            auto scriptName = GetScriptName();
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

        void RunTests(const std::function<void(const std::string&)>& onTestPassed, const std::function<void(const std::string&)>& onTestFailed, const std::function<void(const std::string&)>& onMessage) {
            onMessage("YO WASSUP FROM THE PAPYRUS TEST RUNNER!?");
            InitializeScript();
        }
    };
}
