#pragma once

#include <format>

#pragma warning(push)
#include <RE/Skyrim.h>
#include <RE/C/ConsoleLog.h>
#include <REL/Relocation.h>
#include <SKSE/SKSE.h>
#pragma warning(pop)

#include <SkySpec/Server.h>

namespace SkySpec::Papyrus::Internal {

//    void SkySpec_LogMessage(RE::StaticFunctionTag*, const RE::TESForm* form, std::string text) {
    void SkySpec_LogMessage(RE::StaticFunctionTag*, std::string text) {
        RE::ConsoleLog::GetSingleton()->Print(text.c_str());
        Server::NotifyText(text);
    }

    bool BindPapyrusFunctions(RE::BSScript::Internal::VirtualMachine* vm) {
        vm->RegisterFunction("_LogMessage", "SkySpec", SkySpec_LogMessage);
        return true;
    }
}
