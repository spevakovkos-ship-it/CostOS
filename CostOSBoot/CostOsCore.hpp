#include <iostream>
#include "../BIOS/BIOSCore.hpp"
#include "../Shell/ShellCore.hpp"
#include "../Shell/Shell.hpp"
#include <conio.h>
#include <thread>
#include <chrono>
using namespace std::chrono_literals;
class CostOS {
    private:
        BIOS bios;
        Shell shell;
    public: 

        CostOS(BIOS&bios,Shell&shell,bool tools) : bios(), shell(this->bios,tools) {
        }

        void Boot() {

            
            std::cout << "\nWelcome to Cost OS cpp!\nEnter A to enter to the system, enter DEL to enter intro the BIOS settings\n# ";
            while (true) {
                if (_kbhit()) {
                    int ch = _getch();
                    if (ch == 97 || ch == 65) {
                        int status = shellLoop(shell);
                        if (status == -1) {
                            std::cout << "Bye bye!" << std::endl;
                            return;
                        }
                    } 
                    if (ch == 0 || ch == 224) {
                        int extended = _getch();

                        switch (extended) {
                            case 83:
                                bios.BIOSInterface();
                                 std::cout << "\nWelcome to Cost OS cpp!\nEnter A to enter to the system, enter DEL to enter intro the BIOS settings\n# ";
                        }
                    }
                }
            }
        }
};