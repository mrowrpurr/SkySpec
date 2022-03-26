#pragma once

#include <format>

#include "RE/C/ConsoleLog.h"

namespace SkySpec::Papyrus::Internal {

    void SkySpec_LogMessage(RE::StaticFunctionTag*, const RE::TESForm* form, const std::string& text) {
        // TODO actually associate this with the RUNNING test and send it back...
        // Or, because tests all run at the same time, just use a Server singleton
        // and send the message that way.
        // Actually, let's do it that second way first!
        // Oh. No, let's do both!
         RE::ConsoleLog::GetSingleton()->Print(std::format("LOG '{}' FORM '{}'", text, form->GetName()).c_str());
    }

    bool BindPapyrusFunctions(RE::BSScript::Internal::VirtualMachine* vm) {
        vm->RegisterFunction("LogMessage", "SkySpec", SkySpec_LogMessage);
    }
}
