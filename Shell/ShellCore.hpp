#pragma once
enum class Rights {
    USER,
    ROOT,
    SYSTEM
};
#include <sstream>
#include <string>
#include <format>
#include <iostream>     
#include <unordered_map>
#include <vector>
#include <unistd.h>
#include "../modules/Errors_Table.hpp"

#include "../modules/math_module.hpp"
#include "../BIOS/BIOSCore.hpp"
using std::string;
using Args = std::vector<string>;
class Shell {
    private:
        string command;
        Rights right;
        std::unordered_map<string,void (Shell::*)(const Args&)> commands;
        
    public:
        BIOS bios;
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
                {"errorsMode",&Shell::errorsMode},
                {"fm",&Shell::fm},
                {"animate",&Shell::animateCmd},{"anim",&Shell::animateCmd}
            };
        }
        void switchCommand(string& newCommand) {
            command = newCommand;
        }
        void clear(const Args& args) {
            if (!args.empty()) {
                bios.logError("Clear not need args");
                addError("ShellCore","Clear not need args");

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
                bios.logError("Print require args");
                addError("ShellCore","Print require args");
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
                addError("ShellCore","colorPrint require args");
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
                
            } else {
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
                addError("ShellCore","mathMode not need args");
                return;
            }
            if (!bios.mathEnabled)  {
                bios.logError("Math mode off in BIOS setting");
                addError("ShellCore","Math mode off in BIOS setting");
            }
            else mathInterface(*this);
        }
        void errorsMode(const Args& args) {
            if (!args.empty()) {
                bios.logError("errors table not need args");
                addError("ShellCore","errors table not need args");
                return;
            }
            errorsTableInterface(*this);
        }
        void fmCreate(const Args&args) {
            std::string path = std::format("UserData/{0}",args[0]);
            std::cout << "ARG:  [" << args[0] << "]\n";
            std::cout << "PATH: [" << path << "]\n";
            std::ofstream f(path);

            if (!f.is_open()) {
                throw std::runtime_error("Error opening a file");
            }
            f.close();
        }
        void fmWrite(const Args& args) {
            std::string path = std::format("UserData/{}", args[0]);

            std::ofstream f(path,std::ios_base::app);

            if (!f.is_open()) {
                throw std::runtime_error("Error opening a file");
            }

        for (size_t i = 1; i < args.size(); ++i) {
            if (i > 1)
                f << ' ';

            f << args[i];
            }
             f << '\n'; 
        }
        void fmRead(const Args& args) {
            std::string path = std::format("UserData/{}", args[0]);

            std::ifstream f(path);

            if (!f.is_open()) {
                throw std::runtime_error("Error opening a file");
            }
            std::string line;
            while (std::getline(f,line)) {
                std::cout << line << std::endl;
            }
        }
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
        void animateCmd(const Args& args) {
            if (args.empty())
                return;

            std::string text;

            for (const auto& arg : args)
                text += arg + " ";

            
            animate(text);
            std::cout << std::endl;
            
        }
        void fm(const Args& args) {
            using fmCommand = void(Shell::*)(const Args& args);
            std::unordered_map<std::string,fmCommand> FMCommands = {
                {"create",&Shell::fmCreate},
                {"write",&Shell::fmWrite},
                {"read",&Shell::fmRead}
            };
            auto it = FMCommands.find(args[0]);
            if (it != FMCommands.end()) {
                std::string line;
                for (int i = 1;i < args.size();i++) {
                     if (i > 1)
                        line += ' ';

                    line.append(args[i]);
                }
                Args argsForCmd;
                std::istringstream iss(line);
                std::string arg;
                while (iss >> arg) 
                    argsForCmd.push_back(arg);
                (this->*(it->second))(argsForCmd);
            }
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
                addError("ShellCore","Unknown command");
            }
        }
};


