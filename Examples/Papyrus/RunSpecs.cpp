#include <SkySpec/Runner.h>

int main(int, char* []) {
    auto& runner = SkySpec::Runner::GetSingleton();
    std::cout << "Running Server in Background\n";
    auto thread = runner.RunBackground();


    runner.RunSkseTestSuite("Some Test Suite");


    //    runner.RunPapyrusTestScript("BindMe");


    runner.RunPapyrusTestScript("MyFirstSpec");



    std::cout << "Press any key to exit\n";
    std::cin.get();
    ExitProcess(0);
}
