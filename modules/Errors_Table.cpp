#include "Errors_Table.hpp"
#include "../Shell/ShellCore.hpp"
std::unordered_map<std::string,std::vector<std::string>> errors = {};

void addError(const std::string& module,const std::string& err) {
    errors[module].push_back(err);
}
void getErrors() {
    std::cout << "------------Errors Table------------\n";
     for (const auto& [module, error_list] : errors) {
        std::cout << "Module [" << module << "]:\n";
        for (const auto& err : error_list) {
            std::cout << "  - " << err << "\n";
        }
    }
} 
void errorsTableInterface(Shell&shell) {
    shell.INPUT1 = "ErrorsTable -";
        
    while (true) {
        std::string cmd;
        
        std::cout << shell.INPUT1 << shell.strRight << shell.INPUT2;
        std::getline(std::cin,cmd);

        if (cmd == "getErrors") getErrors();
        else if (cmd == "q" || cmd == "quit" || cmd == "exit") {
            shell.INPUT1 = "OS -";
            return;
        }
        else std::cout << "Unknown command\n";
    }
} 