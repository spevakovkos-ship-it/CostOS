#include "../../modules/CostOSScript/CostOSScript.hpp"
#include "../../Shell/ShellCore.hpp"
#include <fstream>
#include <format>
#include <sstream>
void COSCreateScript(std::vector<std::string>& args, [[maybe_unused]] Shell& shell) {
    if (args.empty()) {
        return;
    }
    std::string name = args[0];
    args.erase(args.begin()); 

    std::string path = std::format("modules/CostOSScript/Scripts/{0}.cost_script", name);
    
    std::ofstream f(path);

    if (!f.is_open()) {
        return;
    }
}


void COSExecuteScript(std::vector<std::string>&args,Shell&shell) {
    if (args.empty()) {
        return;
    }

    std::string name = args[0]; 
    std::string path = std::format("modules/CostOSScript/Scripts/{0}.cost_script",name);
    
    std::ifstream f(path);

    std::string line;
    while (std::getline(f,line)) {
        shell.switchCommand(line);
        shell.executeCommandWithoutLogs();
    }
}


void CostOSScript(Shell&shell) {
    using command = void(*)(std::vector<std::string>&,Shell&shell);

    std::string prevInput = shell.INPUT1;
    shell.INPUT1 = "CostOSScript -";

    static const std::unordered_map<std::string,command> commands = {
        {"create",COSCreateScript},
        {"run",COSExecuteScript},
    };

    while (true) {
        std::string input;
    
        std::cout << shell.INPUT1 << shell.strRight << shell.INPUT2;

        std::getline(std::cin,input);

        std::istringstream iss(input);
        std::string cmd;
        
        iss >> cmd;
        std::vector<std::string> args;
        
        std::string arg;

        while (iss >> arg)
            args.emplace_back(arg);
        
        if (cmd == "exit" || cmd == "q" || cmd == "quit" || cmd == "Exit" ) {
            shell.INPUT1 = prevInput;

            return;
        }
        
        auto it = commands.find(cmd);

        if (it != commands.end()) {
            try {
                it->second(args,shell);
            } catch (std::exception& err) {
                shell.bios.logError(err.what());
                addError("CostOSScript",err.what());
            }
        } else {
            shell.bios.logError("Unknown command");
            addError("CostOSScript","Unknown command");
        }
    }
}