#include "math_module.hpp"
#include "../Shell/ShellCore.hpp"
#include <iostream>
#include <limits>
#include <unordered_map>
#include <sstream>
#include <stdexcept>
#include <random>
int calc(int firstNumber,int secondNumber,char operation) {
    switch (operation) {
        case '+':return firstNumber + secondNumber;
        case '-':return firstNumber - secondNumber;    
        case '*':return firstNumber * secondNumber;    
        case '/':{if (firstNumber == 0 || secondNumber == 0){throw std::runtime_error("Zero divide"); return 0;} return firstNumber / secondNumber; }    
        case '%':{if (firstNumber == 0 || secondNumber == 0){throw std::runtime_error("Zero divide"); return 0;}return firstNumber % secondNumber;  }  

            default:return firstNumber + secondNumber;
        }
    }

void calcCin() {
    int num1,num2;
    char oper;
    std::cout << "Enter a num operation num2: ";
    std::cin >> num1 >> oper >> num2;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Result: " << calc(num1,num2,oper) << '\n';
}
int generateRandomInt(int min,int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(min,max);

    return distr(gen);
}
void generateRandomIntCin() {
    int min,max;
    std::string buffer;

    std::cout << "Enter minimal and maximal numbers: ";
    
    std::getline(std::cin,buffer);

    std::stringstream ss(buffer);
    ss >> min >> max;
    std::cout << "\nResult: " << generateRandomInt(min,max) << std::endl;   
}
void mathClear() {
    #ifdef _WIN32
        std::system("cls");
    #else 
        std::system("clear");
    #endif
}


int reduce(std::vector<int>&vec) {
    int sum = 0;

    for (const auto& value : vec) 
        sum += value;
    

    return sum;
}

void reduceCin() {
    std::string buffer;

    std::cout << "Enter numbers: ";
    std::getline(std::cin,buffer);

    std::vector<int> nums;

    std::stringstream ss(buffer);
    int num;

    while (ss >> num) 
        nums.push_back(num);

    std::cout << "Result: " << reduce(nums) << std::endl;

}

double average(std::vector<int>&vec) {
    if (vec.empty()) throw std::runtime_error("Empty vector in function avg");
    return static_cast<double>(reduce(vec)) / vec.size();
}

void averageCin() {
    std::string buffer;

    std::cout << "Enter numbers: ";
    std::getline(std::cin,buffer);

    std::vector<int> nums;

    std::stringstream ss(buffer);
    int num;

    while (ss >> num) 
        nums.push_back(num);
    
    std::cout << "Result: " << average(nums) << std::endl;
}
void mathInterface(Shell& shell)  {
    using command = void(*)(void);

    shell.INPUT1 = "Math -";

    static const std::unordered_map<std::string,command> mathCommands = {
        {"calc",&calcCin},
        {"clear",&mathClear},
        {"getRandNum",&generateRandomIntCin},
        {"sum",&reduceCin},
        {"avg",&averageCin},{"average",&averageCin},

    };

    while (true) {
        std::string cmd;
    
        std::cout << shell.INPUT1 << shell.strRight << shell.INPUT2;

        std::getline(std::cin,cmd);

        if (cmd == "exit" || cmd == "q" || cmd == "quit" || cmd == "Exit" ) {
            shell.INPUT1 = "OS -";

            return;
        }

        auto it = mathCommands.find(cmd);

        if (it != mathCommands.end()) {
            try {
                it->second();
            } catch (std::exception& err) {
                std::cout << "Error: " << err.what() << std::endl;
            }
        } else {
            std::cout << "Unknown command" << std::endl;
        }
    }
} 