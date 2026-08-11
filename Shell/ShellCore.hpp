#pragma once
enum class Rights {
    USER,
    ROOT,
    SYSTEM
};

#include <string>
#include <iostream>     
#include <unordered_map>
#include <vector>
#include <sstream>
#include "../modules/math_module.hpp"
#include "../BIOS/BIOSCore.hpp"
using std::string;
using Args = std::vector<string>;
class Shell {
    private:
        string command;
        Rights right;
        std::unordered_map<string,void (Shell::*)(const Args&)> commands;
        BIOS bios;
    public:
        string INPUT1;
        const string INPUT2 = "\n# ";
        string strRight;
        Shell(BIOS& biosArg) {
            this->right = Rights::USER;
            this->strRight = "User";
            this->INPUT1 = "OS -";
            this->bios = biosArg;
            commands = {
                {"clear",&Shell::clear},
                {"print",&Shell::print},
                {"colorPrint",&Shell::colorPrint},
                {"mathMode",&Shell::mathMode},
            
            };
        }
        void switchCommand(string& newCommand) {
            command = newCommand;
        }
        void clear(const Args& args) {
            if (!args.empty()) {
                std::cout << "Error: Clear not need args " << std::endl;
                return;
            }
            #ifdef _WIN32
            std::system("cls");
            #else
            std::system("clear");
            #endif
        }

        void print(const Args& args) {
            if (args.empty()) {
                std::cout << "Error: Print require args" << std::endl;
                
                return;
            }
            for (const auto& printValue : args) {
                std::cout << printValue << " ";
            }

            std::cout << "\n";
        }

        void colorPrint(const Args& args) {
            if (args.empty() || args.size() == 1) {
                bios.logError("colorPrint require args");
                return;
            }

            string inputColor = args[0];

            
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
            };
            auto color = colors.find(inputColor);
            if (color == colors.end()) {
                std::cout << "Unknown color: " << inputColor << "  Using default color\n";
                
            }else {
                std::cout << color->second;
            }   
            for (int i = 1;i < args.size();i++) {
                std::cout << args[i] << " ";
            }

            std::cout << "\033[0m" << std::endl;
        }
        void mathMode(const Args& args) {
            if (!args.empty()) {
                bios.logError("mathMode not need args");
                
                return;
            }
            if (!bios.mathEnabled) 
                std::cout << "Math mode off in BIOS setting" << std::endl;
            else mathInterface(*this);
        }
        void executeCommand() {
            std::istringstream iss(command);
            std::string cmd;
            Args args;
            
            iss >> cmd;

            string arg;
            
            while (iss >> arg) 
                args.push_back(arg);

        
            auto it = commands.find(cmd);

            if (it != commands.end()) {
                (this->*(it->second))(args);
            }
            else {
                bios.logError("Unknown command");
              
            }
        }
};


