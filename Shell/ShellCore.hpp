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
#include <filesystem>
#include <thread>
#include <chrono>
#include <functional>
#include <algorithm>

#include "../modules/Errors_Table.hpp"
#include "../pkg/counter/counter.hpp"
#include "../modules/math_module.hpp"
#include "../BIOS/BIOSCore.hpp"
#include "../pkg/colorful_console/colorful_console.hpp"
#include "../pkg/fm20/fm20.hpp"
using std::string;
struct Macro {
    string name;
    
    std::vector<string> args;


};
using namespace std::chrono_literals;
class Shell {
    private:
        string command;
        Rights right;
        std::unordered_map<string,void (Shell::*)(const Args&)> commands;
        std::unordered_map<string,void (*)(const Args&)> packageCommands;
        std::vector<Macro> macroses;
        inline static const std::unordered_map<string,bool*> packages = {
            {"counter", &counterDownloaded},
            {"colorful_console",&colorful_consoleDownloaded},
            {"fm20",&fm20Downloaded}
        };
    public:
        BIOS bios;
        string INPUT1 = "OS -";
        string INPUT2 = "# ";
        string strRight;

        Shell(BIOS& biosArg,bool tools) {
            this->right = Rights::USER;
            this->strRight = "User\n";
            if (tools) {
                this->right = Rights::SYSTEM;
                    this->strRight = "SYSTEM\n";

            }
            this->bios = biosArg;
            commands = {
                {"clear",&Shell::clear},
                {"print",&Shell::print},
                {"colorPrint",&Shell::colorPrint},
                {"mathMode",&Shell::mathMode},
                {"errorsMode",&Shell::errorsMode},
                {"fm",&Shell::fm},
                {"animate",&Shell::animateCmd},{"anim",&Shell::animateCmd},
                {"costos_pkg",&Shell::costosPkg},
                {"syscall",&Shell::syscall},
                {"createMacro",&Shell::createMacro},
                {"repeat",&Shell::repeat},
                {"renameMacro",&Shell::renameMacro}
            };
            packageCommands = {
                {"counter",&counter},
                {"colorful_console",&colorful_console},
                {"fm20",&fm20}
            };
            macroses = {};
            static const std::unordered_map<std::string,bool*> packages = {
                {"counter",&counterDownloaded},
                {"colorful_console",&colorful_consoleDownloaded},
                {"fm20",&fm20Downloaded}
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
        void SCchangeINPUT2(const Args& args) {
            if (args.empty()) {
                std::cout << "SYSTEM ERROR: SYSCALL CHANGEINPUT NEED A ARGUMENTS" << std::endl;
                return;
            }
            INPUT2 = (args[0] + ' ');
        }
        void SCchangeINPUT1(const Args& args) {
            if (args.empty()) {
                std::cout << "SYSTEM ERROR: SYSCALL CHANGEINPUT NEED A ARGUMENTS" << std::endl;
                return;
            }
            INPUT1 = (args[0] + " -");
        }
        void SCchangeColor(const Args& args) {
            if (args.empty()) {
                std::cout << "SYSTEM ERROR: SYSCALL CHANGECOLOR NEED A ARGUMENTS" << std::endl;
                return;
            }
            std::cout << "\033["<<args[0] << "m";
        }
        void SCcreateMacro(const Args& args) {
            if (args.size() < 2) {
                std::cout << "SYSTEM ERROR: SYSCALL CREATEMACRO NEED ARGUMENTS\n";
                return;
            }
            Macro m;

            m.name = args[0];
            
            if (args[1] == "syscall")
                return;

            std::vector<string> macroCommand(
                args.begin() + 1,
                args.end()
            );
            m.args = macroCommand;
            macroses.push_back(m);
        } 
        void createMacro(const Args& args) {
            if (args.size() < 2) {
                bios.logError("createMacro need args");
                addError("ShellCore","createMacro need args");
                return;
            }
            Macro m;

            m.name = args[0];
            
            if (args[1] == "syscall")
                return;

            std::vector<string> macroCommand(
                args.begin() + 1,
                args.end()
            );
            m.args = macroCommand;
            macroses.push_back(m);
        }
        void executeMacro(const Args& args) {
             if (args.size() < 1) {
                bios.logError("executeMacro need args");
                addError("ShellCore","executeMacro need args");
                return;
            }
            string macroName = args[0];
            bool found = false;
            Macro mac;

            for (const auto& m: macroses) {
                if (m.name == macroName) {
                    mac = m;
                    found = true;
                    break;
                }

            }
            if (found) {
                string resCommand;

                int argIndex = 1;
                
                for (auto& v : mac.args) {
                    if ((v.find("-arg")) != std::string::npos) {
                        if (args.size() < argIndex) {
                            bios.logError("Arguments size is not for all macro args");
                            addError("ShellCore","Arguments size is not for all macro args");
                            return;
                        }
                        v.replace(v.find("-arg"), 4, args[argIndex]);
                        argIndex++;
                    }
                    resCommand += v;
                    resCommand += " ";
                }

                this->switchCommand(resCommand);
                this->executeCommand();
            }   else {
                bios.logError("Macro not found");
                addError("ShellCore","Macro not found");
            }
        }
     
        void SCrenameMacro(const Args& args) {
            if (args.empty()) {
                std::cout << "SYSTEM ERROR: SYSCALL RENAMEMACRO NEED A ARGUMENT\n";
                return;
            }
            string oldName = args[0];
            string newName = args[1];

            for (auto& m : macroses) {
                if (m.name == oldName) {
                    m.name = newName;
                    break;  
                }
            } 
        }
        void renameMacro(const Args& args) {
            if (args.size() < 2) {
                bios.logError("renameMacro need args");
                addError("ShellCore","renameMacro need args");
                return;
            }
            
            string oldName = args[0];
            string newName = args[1];

            for (auto& m : macroses) {
                if (m.name == oldName) {
                    m.name = newName;
                    break;  
                }
            } 
        }
        void repeat(const Args& args ){
            if (args.size() <= 1) {
                bios.logError("Repeat need args");
                addError("ShellCore","Repeat need args");
                return;
            }
            int count = std::stoi(args[0]);

            string resCommand;

            for (int i = 1;i < args.size();++i) {
                resCommand += args[i];
                resCommand += " ";
            }
            this->switchCommand(resCommand);
            for (int i = 0;i < count;++i) {
                this->executeCommand();
            }
        }
        void syscall(const Args& args) {
            if (right != Rights::SYSTEM) {
                bios.logError("Permission denied");
                addError("ShellCore","Permission denied");
                return;
            }
            using syscallCommands = void(Shell::*)(const Args&);
            static const std::unordered_map<std::string,syscallCommands> commandsForsyscall = {
                {"changeINPUT2",Shell::SCchangeINPUT2},
                {"changeINPUT1",Shell::SCchangeINPUT1},
                {"changeCOLOR",Shell::SCchangeColor},
                {"createMACRO",Shell::SCcreateMacro},
                {"renameMACRO",Shell::SCrenameMacro},
                {"executeMACRO",Shell::executeMacro}
            };
            auto it = commandsForsyscall.find(args[0]);
            if (it != commandsForsyscall.end()) {
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
            };
        }
        void pkgInstall(const Args& args) {
           
            if (args.empty()) {
                bios.logError("Package name required");
                addError("ShellCore","Package name required");
                return;
            }

            std::string packageName = args[0];

            auto it = packages.find(packageName);
            if (*it->second == true) {
                bios.logError("Package now installed");
                addError("ShellCore","Package now installed");
                return;
            } 
            if (it != packages.end()) {
                for (int i = 0;i < 100;i+=10) {
                    std::cout << "\n[PackagesManager] Resolving promises " << i <<"...";
                    std::this_thread::sleep_for(500ms);
                }
                for (int i = 0;i < 100;i+=10) {
                    std::cout << "\n[PackagesManager] Getting a premission from BIOS  " << i <<"...";
                    std::this_thread::sleep_for(100ms);
                }
                for (int i = 0;i < 100;i+=10) {
                    std::cout << "\n[PackagesManager] Installing  " << i <<"...";
                    std::this_thread::sleep_for(750ms);
                }
                *it->second = true;
                std::cout << "\n[PackagesManager] Done " << packageName <<" installed\n";
            }
        }
        void pkgCall(const Args& args) {
            if (args.empty()) {
                bios.logError("PKG call need arguments");
                addError("ShellCore","PKG call need arguments");
                return;
            }
            std::string name = args[0]; 
            Args newArgs;
            std::string arg;
            for (int i = 1;i < args.size();++i) {
                arg = args[i];
                newArgs.push_back(arg);
                arg = "";
            }
            auto it = packageCommands.find(name);
            if (it != packageCommands.end()) {
                std::invoke(it->second,newArgs);
            } else {
                bios.logError("Package not found");
                addError("ShellCore","Package not found");
            }
        }
        void pkgRemove(const Args& args) {
            if (args.empty()) {
                bios.logError("PKG remove need arguments");
                addError("ShellCore","PKG remove need arguments");
                return;
            }
            string name = args[0];

            auto it = packages.find(name);

            if (it != packages.end()) {
                for (int i = 0;i < 100;i+=10) {
                    std::cout << "\n[PackagesManager] DeResolving promises " << i <<"...";
                    std::this_thread::sleep_for(500ms);
                }
                for (int i = 0;i < 100;i+=10) {
                    std::cout << "\n[PackagesManager] Getting a premission from BIOS  " << i <<"...";
                    std::this_thread::sleep_for(100ms);
                }
                for (int i = 0;i < 100;i+=10) {
                    std::cout << "\n[PackagesManager] Removeing " << i <<"...";
                    std::this_thread::sleep_for(750ms);
                }
                *it->second = false;
                std::cout << "\n[PackagesManager] Done " << name <<" removed\n";
            }
        }
        void costosPkg(const Args& args) {
            using pkgCommand = void(Shell::*)(const Args& args);
            std::unordered_map<std::string,pkgCommand> pkgCommands = {
                {"install",&Shell::pkgInstall},
                {"call",&Shell::pkgCall},
                {"remove",&Shell::pkgRemove}
            };
            auto it = pkgCommands.find(args[0]);
            if (it != pkgCommands.end()) {
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
            };
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
            std::ofstream f("UserData/"+args[0]);

            
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
        ~Shell() {
            std::cout << "\033[0m";
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
        void fmRemove(const Args& args) {
           if (args.empty()) {
                bios.logError("Fm remove need args");
                addError("File manager","Fm remove need args");

           }
            std::filesystem::path path = std::format("UserData/{}", args[0]);
            try {
                if (std::filesystem::remove(path)) {
                    std::cout << "File " << path << " deleted" << std::endl;
                } else {
                    std::cout << "File " << path << " not found" << std::endl;
                }
            } catch (const std::filesystem::filesystem_error& e) {
                bios.logError(e.what());
                addError("File manager",e.what());

            }
        }
        void fmDir(const Args& args) {
            std::filesystem::path catalogPath = "UserData"; 
            try {
                if (std::filesystem::exists(catalogPath) && std::filesystem::is_directory(catalogPath)) {
                    for (const auto& entry : std::filesystem::directory_iterator(catalogPath)) {
                        if (entry.is_regular_file()) {
                            std::cout << entry.path().filename() << std::endl;
                        }
                    }
                } else {
                    std::cout << "Directory does not exist.\n";
                }
            } catch (const std::filesystem::filesystem_error& e) {
                bios.logError(e.what());
                addError("File manager",e.what());
            }
        }
        void animate(const std::string& text) {
            std::cout << "\x1b[?25l                                         ";
    
            for (const char& v : text) {
                std::this_thread::sleep_for(100ms);
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
                {"read",&Shell::fmRead},
                {"dir",&Shell::fmDir},
                {"remove",&Shell::fmRemove}
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
                try {
                    (this->*(it->second))(args);
                } catch (std::exception& err ){
                    bios.logError(err.what());
                    addError("ShellCore",err.what());
                    return;
                }
            }
            else {
    
                bool found = false;
                Macro mac;
                mac.args = args;
                for (auto& m : macroses) {
                    if (m.name == cmd) {
                        found = true;
                        mac.name = m.name;
                    }
                }
                if (found) {
                    Args macroFullCommand;
                    macroFullCommand.push_back(mac.name);
                    for (auto& arg : mac.args) {
                        macroFullCommand.push_back(arg);
                    }
                    executeMacro(macroFullCommand);
                } else {
                    bios.logError("Unknown command");
                    addError("ShellCore","Unknown command");
                }
            }
        }
};


