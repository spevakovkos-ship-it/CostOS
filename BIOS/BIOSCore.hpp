#pragma once
#include "../modules/Errors_Table.hpp"
#include <string>
#include <unordered_map>
#include <fstream>
#include <exception>
#include <variant>
#include <limits>
#include <stdexcept>
#include <sstream>
#include <iostream>

enum class configPars {
    LOG_ERRORS,
    MATH_ENABLED
};

class BIOS {
    private:
        
        using BoolPtr = bool BIOS::*;
        using IntPtr  = int BIOS::*;
        using FieldPtr = std::variant<BoolPtr, IntPtr>;
        std::string PATH = "BIOS/Config.conf";
    public:
        bool mathEnabled = true;
        bool logErrors = true; 
        BIOS() {
            try {getConfig();} catch (std::exception&) {
                try {loadConfig(PATH);} catch (std::exception&) {
                    addError("BIOS","Config corrupted used SB");
                    PATH = "CostOS/BIOS/Config.conf";
                    try {loadConfig(PATH);} catch (std::exception&) {
                        std::cout << "\nError loading Config.conf, Please ensure that you are in the project folder." << std::endl;
                        std::terminate();   
                    }
                }
                addError("BIOS","Config corrupted used SB");

                 
            }
        }
        void loadConfig(const std::string & path) {
            std::ofstream conf(path,std::ios_base::out);

            if (!conf.is_open())
                throw std::runtime_error("Error: opening a Config.conf failed");
            
            conf << std::boolalpha; 
            conf << "mathEnabled:" << mathEnabled;
            conf << "\nlogErrors:" << logErrors;

        }
        BIOS getConfig() {
            static const std::unordered_map<std::string, FieldPtr> configMap = {
                {"mathEnabled", &BIOS::mathEnabled},
                {"logErrors",   &BIOS::logErrors}
            };

            std::ifstream file("BIOS/Config.conf");

            if (!file.is_open()) 
                throw std::runtime_error("Error: Opening a Config.conf failed");
            
            std::string line;

            while (std::getline(file, line)) {
                size_t colonPos = line.find(':');

                if (colonPos != std::string::npos) {
                    std::string key = line.substr(0, colonPos);
                    std::string valueStr = line.substr(colonPos + 1);
                        
                    auto it = configMap.find(key);
                    if (it != configMap.end()) {
                        
                        std::visit([this, &valueStr](auto&& field_ptr) {
                            using T = std::decay_t<decltype(field_ptr)>;
                            
                            if constexpr (std::is_same_v<T, BoolPtr>) {
                                this->*field_ptr = (valueStr == "true");
                            } 
                            else if constexpr (std::is_same_v<T, IntPtr>) {
                                    this->*field_ptr = std::stoi(valueStr);
                            }
                        }, it->second);

                    }
                }
            }
            return *this;
        }
        void logError(const char *err) {
            if (logErrors == false) {
                return; 
            }
            std::cout << "Error: " << err << std::endl; 
        } 
        void logError(const std::string &err) {
            if (logErrors == false) {
                return; 
            }
            std::cout << "Error: " << err << std::endl; 
        } 
        void BIOSInterface() {
            std::cin.clear();
    
            if (std::cin.rdbuf()->in_avail() > 0) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            while (true) {
                std::cout << std::boolalpha << "\nBIOS setting V1.0\nlogErrors:" << logErrors << "\nmathEnabled:" << mathEnabled << "\n# ";
                
                static const std::unordered_map<std::string, BoolPtr> boolMap = {
                    {"mathEnabled", &BIOS::mathEnabled},
                    {"logErrors",   &BIOS::logErrors}
                };

                static const std::unordered_map<std::string, IntPtr> intMap = {
                };

                std::string input;
                std::getline(std::cin,input);

                std::stringstream ss(input);

                bool value;
                std::string par;

                ss >> par;

                if (par == "exit" || par == "q" || par == "quit" || par == "execute-[1]") {
                    loadConfig(PATH);
                    break;
                } 
                auto itBool = boolMap.find(par);
                if (itBool != boolMap.end()) {
                    bool value;
                    if (ss >> std::boolalpha >> value) {
                        BoolPtr ptr = itBool->second;
                        this->*ptr = value;
                    }
                    loadConfig(PATH);
                    std::cout << "Reboot system to apply the changes\n" << std::endl;
                    continue; 
                }

                auto itInt = intMap.find(par);
                if (itInt != intMap.end()) {
                    int value;
                    if (ss >> value) {
                        IntPtr ptr = itInt->second;
                        this->*ptr = value; 
                    }
                    loadConfig(PATH);
                    std::cout << "Reboot system to apply the changes" << std::endl;
                    continue;
                }

                logError("Unknown parameter");
            }

            };
        };
 