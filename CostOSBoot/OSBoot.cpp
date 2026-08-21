#include "CostOsCore.hpp"
#include <unistd.h>
void animate(const std::string& text) {
    std::cout << "\x1b[?25l                                         ";
    
    for (const char& v : text) {
        usleep(100000);
        if (v == '\n') {
            std::cout << "\n                                        ";
        } else {
            std::cout << v ;
        }
    }
    std::cout <<  "\x1b[?25h";
}

int main(int argc,char *argv[]) {
    bool toolsMode = false;

    if (argc >= 2) {
        std::string arg = argv[1];

        if (arg == "-TOOLS" || arg == "-tools") {
            toolsMode = true;
        }
    }
    BIOS bios;
    Shell s(bios,toolsMode);
    CostOS os(bios,s,toolsMode);    
    animate("Welcome to CostOS! \n now loading started... \n progress:[###################################]");
    std::cout << std::endl;
    os.Boot();
    return 0;
}