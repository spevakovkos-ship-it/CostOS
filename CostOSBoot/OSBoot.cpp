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

int main() {
    BIOS bios;
    Shell s(bios);
    CostOS os(bios,s);    
    animate("Welcome to CostOS! \n now loading started... \n progress:[###################################]");
    std::cout << std::endl;
    os.Boot();
    return 0;
}