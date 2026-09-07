#include "../../modules/CostOSScript/CostOSScript.hpp"
#include "../../Shell/ShellCore.hpp"
#include <fstream>
#include <format>
#include <sstream>
#include <filesystem>
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
void COSWriteToScript(std::vector<std::string>&args,Shell&shell) {
    if (args.empty()) {
        return;
    }
    std::string name = args[0]; 
    std::string path = std::format("modules/CostOSScript/Scripts/{0}.cost_script",name);
    std::string text;
    int i = 0;
    for (const auto& textPart : args) {
        if (i == 0) {
            ++i;
            continue;
        }
        text.append(textPart);
        text.append(" ");
    }

    std::ofstream f(path,std::ios_base::app);
    

    f << text;
    f << '\n';
}   
void COSReadScript(std::vector<std::string>&args,Shell&shell) {
    if (args.empty() )return;

    std::string name = args[0]; 
    std::string path = std::format("modules/CostOSScript/Scripts/{0}.cost_script", name);
    std::ifstream in(path);

    std::string line;
    while (std::getline(in,line))
        std::cout << line << "\n";
}   
void COSRemoveLineInScript(std::vector<std::string>& args, Shell& shell) {
    if (args.empty()) {
        return;
    }  
    std::string name = args[0]; 
    std::string path = std::format("modules/CostOSScript/Scripts/{0}.cost_script", name);
    std::ifstream in(path);


    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) {
        lines.push_back(line);
    }
    in.close();

    if (lines.empty()) {
        return;
    }

    lines.pop_back();

    std::ofstream out(path, std::ios::trunc); 
    if (!out.is_open()) {
        return;
    }

    for (const auto& l : lines) {
        out << l << '\n';
    }
    out.close();
}
void COSHelp(std::vector<std::string>&args,Shell&shell) {
    std::cout <<"| Command | Arguments | Description |\n";
    std::cout <<"|---|---|---|\n";
    std::cout <<"|\033[32m`create`\033[0m| `<name of file>` | Creates a file in folder `Scripting` that store you script\n";
    std::cout <<"|\033[32m`run`\033[0m | `<name of file>` | Run the script\n";
    std::cout <<"|\033[32m`write`\033[0m | `<name of file> <text> ...` | Write text inside script\n";
    std::cout <<"|\033[32m`removeLine`\033[0m | `<name of file>` | Remove last line in script\n";
    std::cout <<"|\033[32m`read`\033[0m | `<name of file>` | Read text inside script\n";
    std::cout <<"|\033[32m`help`\033[0m | - | Help\n";
}
void CostOSScript(Shell&shell) {
    using command = void(*)(std::vector<std::string>&,Shell&shell);

    std::string prevInput = shell.INPUT1;
    shell.INPUT1 = "CostOSScript -";

    static const std::unordered_map<std::string,command> commands = {
        {"create",COSCreateScript},
        {"run",COSExecuteScript},
        {"write",COSWriteToScript},
        {"removeLine",COSRemoveLineInScript},
        {"read",COSReadScript},
        {"help",COSHelp}
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