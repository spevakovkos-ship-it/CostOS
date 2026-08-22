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
    bool skipLoading = false;
    const std::unordered_map<std::string,bool*> flags = {
        {"-tools",&toolsMode},{"-TOOLS",&toolsMode},
        {"-skipLoading",&skipLoading},
    };
    if (argc >= 2) {
        for (int i = 0;i < argc;++i) {
            std::string arg = argv[i];

            auto it = flags.find(arg);

            if (it != flags.end()) {
                *it->second = true;
            }
        }
    }
    BIOS bios;
    Shell s(bios,toolsMode);
    CostOS os(bios,s,toolsMode);    
    if (!skipLoading) animate("Welcome to CostOS! \n now loading started... \n progress:[###################################]");
    std::cout << std::endl;
    os.Boot();
    return 0;
}