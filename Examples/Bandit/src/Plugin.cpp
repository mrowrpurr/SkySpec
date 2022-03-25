#include <SKSE/SKSE.h>
#include <RE/C/ConsoleLog.h>
#include <SkySpec/Server.h>

extern "C" __declspec(dllexport) bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* skse) {
    SKSE::Init(skse);
    SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* event){
        if (event->type == SKSE::MessagingInterface::kDataLoaded) {
            RE::ConsoleLog::GetSingleton()->Print("Hi from this Bandit example! TODO: call the server!");
            SkySpec::Server::RegisterTestSuite("My Cool Test Suite", [](){
                RE::ConsoleLog::GetSingleton()->Print("RAN MY COOL TEST SUITE!");
                SkySpec::Server::NotifyText("This is text from My Cool Test Suite!");
                SkySpec::Server::NotifyTestPassed("Something Passed!");
                SkySpec::Server::NotifyTestFailed("Something Failed!");
                RE::ConsoleLog::GetSingleton()->Print("DID STUFF PRINT OUT?");
            });
        }
    });
    return true;
}

extern "C" __declspec(dllexport) bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface*, SKSE::PluginInfo* info) {
    info->infoVersion = SKSE::PluginInfo::kVersion;
    info->name = "SkySpec_Example_Bandit";
    info->version = 1;
    return true;
}

extern "C" __declspec(dllexport) constinit auto SKSEPlugin_Version = [](){
    SKSE::PluginVersionData version;
    version.PluginName("SkySpec_Example_Bandit");
    version.PluginVersion({ 0, 0, 1 });
    version.CompatibleVersions({ SKSE::RUNTIME_LATEST });
    return version;
}();
