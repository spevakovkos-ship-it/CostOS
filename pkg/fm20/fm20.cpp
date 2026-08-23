#include "fm20.hpp"
#include <format>
#include <fstream>
#include <iostream>
#include <cctype>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <functional>








void wc_fn(const std::vector<std::string>& args){
    if (!fm20Downloaded) return;
    const std::string fileName = args[0];
    std::string path = std::format("UserData/{0}",fileName);

    std::ifstream f(path);

    int lines = 0,chars = 0,words = 0;

    std::string buffer;

    while (std::getline(f, buffer)) {
        lines++;

        chars += static_cast<int>(buffer.size());

        bool inWord = false;

        for (char c : buffer) {
            if (std::isspace(static_cast<unsigned char>(c))) {
                inWord = false;
            }
            else if (!inWord) {
                words++;
                inWord = true;
            }
        }
    }

    std::cout << "Lines: " << lines << "\nChars: " << chars << "\nWords: " << words << std::endl; 
}
void find_fn(const std::vector<std::string>& args) {
    if (!fm20Downloaded) return;

    std::string fileName = args[0];
    for (const auto& entry : std::filesystem::directory_iterator("UserData")) {
        if (entry.path().filename() == fileName) {
            std::cout << entry.path().string() << '\n';
        }
    }
};
void grep_fn(const std::vector<std::string>& args) {
    if (!fm20Downloaded) return;

    std::string fileName = args[0];
    std::string text = args[1];
    std::ifstream file("UserData/" + fileName);

    std::string line;

    while (std::getline(file, line)) {
        if (line.find(text) != std::string::npos) {
            std::cout << line << '\n';
        }
    }
};
void reverse_fn(const std::vector<std::string>& args){
    if (!fm20Downloaded) return;

    std::string fileName = args[0];
    std::ifstream file("UserData/" + fileName);

   

    std::string content(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    std::reverse(content.begin(), content.end());

    std::cout << content;
};

void head_fn(const std::vector<std::string>& args) {
    if (!fm20Downloaded) return;

    std::string fileName = args[0];
    int count = std::stoi(args[1]); 
    std::ifstream file("UserData/" + fileName);

    if (!file) return;
    int line = 0;
    std::string buffer;
    while (getline(file,buffer) && line < count) {
        line++;
        std::cout << buffer << std::endl;
    }
}; 
void tail_fn(const std::vector<std::string>& args) {
    if (!fm20Downloaded) return;

    std::string fileName = args[0];
    int count = std::stoi(args[1]); 
    std::ifstream file("UserData/" + fileName);

    if (!file) {
        return;
    }

    std::vector<std::string> lines;
    std::string buffer;

    while (std::getline(file, buffer)) {
        lines.push_back(buffer);
    }

    int start = std::max(0, static_cast<int>(lines.size()) - count);

    for (int i = start; i < lines.size(); i++) {
        std::cout << lines[i] << '\n';
    }
}









void fm20(const std::vector<std::string>& args) {
    if (!fm20Downloaded) return;

    const static std::unordered_map<std::string, std::function<void(const std::vector<std::string>&)>> commandsForFm20 = {
        {"reverse",reverse_fn},
        {"head",head_fn},
        {"tail",tail_fn},
        {"wc",wc_fn},
        {"find",find_fn},
        {"grep",grep_fn},

    };
    if (args.empty() ) {
        return;
    }
    std::string line;
    for (int i = 1;i < args.size();i++) {
        if (i > 1)
            line += ' ';

        line.append(args[i]);
    }
    std::vector<std::string> argsForCmd;
    std::istringstream iss(line);
    std::string arg;
    while (iss >> arg) 
        argsForCmd.push_back(arg);

    auto it = commandsForFm20.find(args[0]);

    if (it != commandsForFm20.end()) {
        std::invoke(it->second,argsForCmd);
    }
}





































