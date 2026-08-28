#pragma once

#include "ShellCore.hpp"
#include <cstring>


int shellLoop(Shell& shell) {
    bool running = true;
    bool first = true;

    shell.INPUT1 = "OS -";

    while (running) {
        string command;

        if (!first)std::cout << shell.INPUT1 << shell.strRight << shell.INPUT2;
        else std::cout << '\n' <<shell.INPUT1 << shell.strRight << shell.INPUT2;
        std::getline(std::cin,command);

        if (command == "q" || command == "quit" || command == "exit" || command == "Exit") {
            running = false;
            return -1;
        }

        shell.switchCommand(command);
        shell.executeCommand();
        first = false;
    }
    return 0;
}