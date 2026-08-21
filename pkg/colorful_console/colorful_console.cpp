#include "colorful_console.hpp"
#include <unordered_map>
#include <iostream>
void colorful_console(const std::vector<std::string>& args) {
    using namespace std;
    if (args.empty()) {
        return;
    }
    string color = args[0];


    static const std::unordered_map<string,string> colors = {
        {"Red","\033[31m"},
        {"Green","\033[32m"},  
        {"Yellow","\033[33m"},  
        {"Blue","\033[34m"},  
        {"Cyan","\033[36m"},
        {"Magenta","\033[35m"},
        {"Light-black","\033[90m"},
        {"Light-red","\033[91m"},
        {"Light-green","\033[92m"},
        {"Light-yellow","\033[93m"},
        {"Light-blue", "\033[94m"},
        {"Light-magenta", "\033[95m"},
        {"Light-cyan", "\033[96m"},
        {"Light-white", "\033[97m"},
        {"reset","\033[0m"}
    };


    auto it = colors.find(color);

    if (it != colors.end()) {
        cout << it->second;
    }
}
