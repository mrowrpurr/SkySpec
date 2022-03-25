#include <functional>
#include <string>

namespace SkySpec {
    class PapyrusSpec {
        std::string _scriptName;

    public:
        PapyrusSpec(const std::string& scriptName) : _scriptName(scriptName) {}

        void InitializeScript() {

        }

        void RunTests(const std::function<void(const std::string&)>& onTestPassed, const std::function<void(const std::string&)>& onTestFailed, const std::function<void(const std::string&)>& onMessage) {
            onMessage("YO WASSUP FROM THE PAPYRUS TEST RUNNER!?");
        }
    };
}
