#include "Errors_Table.hpp"
#include "../Shell/ShellCore.hpp"
std::unordered_map<std::string,std::vector<std::string>> errors = {};

void addError(const std::string& module,const std::string& err) {
    errors[module].push_back(err);
}
void getErrors(const std::vector<std::string> args) {
    if (errors.empty())  {
        std::cout << "Errors Table are empty" << std::endl;
        return;
    }
    std::cout << "------------Errors Table------------\n";
     for (const auto& [module, error_list] : errors) {
        std::cout << "Module [" << module << "]:\n";
        for (const auto& err : error_list) {
            std::cout << "  - " << err << "\n";
        }
    }
} 

void errClear(const std::vector<std::string> args) {
    if (args.empty()) {
        std::system("cls");
        return;
    }
    if (args[0] == "-table") errors.clear();
    else std::system("cls");
}
void errorsTableInterface(Shell&shell) {
    std::string prevInput = shell.INPUT1;
    shell.INPUT1 = "ErrorsTable -";
    
    static const std::unordered_map<std::string,std::function<void(const std::vector<std::string>)>> cmds = {
        {"getErrors",getErrors},
        {"clear",errClear}
    } ;
    while (true) {
        std::string input;
        
        std::cout << shell.INPUT1 << shell.strRight << shell.INPUT2;
        std::getline(std::cin,input);

        if (input == "q" || input == "quit" || input == "exit") {
            shell.INPUT1 = prevInput;
            return;
        }

        std::istringstream iss(input);
        std::string cmd;

        std::vector<std::string> args;

        iss >> cmd;

        std::string arg;
        while (iss >> arg) 
            args.emplace_back(arg);
        
        auto it = cmds.find(cmd);

        if (it != cmds.end()) {
            std::invoke(it->second,args);
        } else {
            std::cout << "Unknown command" << std::endl;
            addError("ErrorsTable","Unknown command");
        }
    }
} 