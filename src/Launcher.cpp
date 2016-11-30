#include "Launcher.hpp"

int main(int argc, char* argv[]) {
    try {
        if (argc < 1) { // First argument always program call
            throw "Error in Launcher; illegal number of args.";
        } else if (argc == 1) {
            // Do things if user runs the program normally, like...
            Controller mainController();
        } else {
            // Otherwise parse possible flags.
            std::vector<std::string> argList(argv, argv + argc);
            for (int i = 1; i < argc; i++) {
                if (argList[i] == "-t" || argList[i] == "--test") {
                    // return Tests::runTests(); FIXME: Launcher.cpp:15:28: error: ‘Tests’ has not been declared
                }
                if (argList[i] == "-h" || argList[i] == "--help") {
                    std::cout << "This is the help string." << std::endl
                    << "Execute with -t or --test to run the tests. Execute without parameters to start up the GUI. Execute with -h or --help to view this text.";
                }
            }
        }
        

    } catch (const char* msg) {
        std::cerr << "UNCAUGHT RUNTIME ERROR: " << msg << std::endl;
    }
    
    return 0;
}