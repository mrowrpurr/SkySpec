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

        void SkySpec_FailTest(RE::BSScript::Internal::VirtualMachine* vm, RE::VMStackID stackID, RE::StaticFunctionTag*) {
        Server::NotifyTestFailed("Some test failed! But we need more info!");

        auto stack = vm->allRunningStacks.find(stackID)->second;
        auto* frame = stack->top->previousFrame->previousFrame;
        auto* fn = skyrim_cast<RE::BSScript::Internal::ScriptFunction*>(frame->owningFunction.get());
        auto fileName = fn->GetSourceFilename().c_str();
        uint32_t lineNumber;
        bool worked = fn->TranslateIPToLineNumber(frame->instructionPointer, reinterpret_cast<uint32_t&>(lineNumber));
        Server::NotifyText(std::format("WORKED? '{}' FILE:'{}' LINENO:'{}'", worked, fileName, lineNumber));
    }

    bool BindPapyrusFunctions(RE::BSScript::Internal::VirtualMachine* vm) {
        vm->RegisterFunction("_LogMessage", "SkySpec", SkySpec_LogMessage);
        vm->RegisterFunction("_FailAssertion", "SkySpec", SkySpec_FailTest);
        return true;
    }
}
