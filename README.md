CostOS cpp






A hobbyist CLI operating system emulator written in C++. CostOS features a modular architecture that separates the virtual BIOS configuration layer, the core system interface, and specialized functional environments.

Key Features
Dual-Layer Architecture: Separates BIOS configuration and Shell execution environments.
Interactive Bootloader: Allows the user to boot into the OS or access the BIOS setup using the DEL key.
Virtual BIOS Configurator: Reads and writes system variables to a persistent configuration file.
Advanced CLI Shell: Supports command tokenization, argument parsing, role-based access levels (USER, ROOT, SYSTEM), and ANSI color formatting.
Dedicated Math Subshell: Provides an isolated mathematical environment with its own command set and prompt.
Errors Table: Centralized runtime error storage organized by module.
Modular Design: BIOS, Shell, Math, and Error handling are separated into independent components.
Built-in Shell Commands
Core OS Commands
Command	Arguments	Description
clear	—	Clears the terminal screen buffer (cls / clear).
print	<text1> [text2] ...	Prints one or more space-separated arguments.
colorPrint	<Color> <text1> ...	Prints text using ANSI colors such as Red, Green, and Cyan.
mathMode	—	Launches the Math subshell if it is enabled in BIOS settings.
errorsTable	—	Launches the Errors Table interface.
Math Subshell Commands

Accessible through mathMode.

Command	Arguments	Description
calc	—	Evaluates a basic infix mathematical expression using +, -, *, /, and %.
getRandNum	—	Generates a pseudo-random integer within a custom range.
sum	—	Accepts a space-separated list of integers and returns their sum.
avg / average	—	Accepts a space-separated list of integers and calculates the mean.
clear	—	Clears the Math subshell terminal workspace.
exit / q / quit	—	Exits the Math environment and restores the main OS prompt.
Errors Table Commands

Accessible through errorsTable.

Command	Arguments	Description
getErrors	—	Displays all registered errors grouped by module.
exit / q / quit	—	Exits the Errors Table interface and restores the main OS prompt.
Errors Table

The Errors Table is a centralized runtime error storage system.

Errors are stored in an std::unordered_map, where each module has its own list of errors:

std::unordered_map<std::string, std::vector<std::string>> errors;

Errors can be registered using:

void addError(const std::string& module, const std::string& err);

For example:

addError("BIOS", "Failed to load configuration");
addError("Shell", "Unknown command");
addError("Math", "Zero divide");

The resulting table can be displayed through the getErrors command:

------------Errors Table------------
Module [BIOS]:
  - Failed to load configuration

Module [Shell]:
  - Unknown command

Module [Math]:
  - Zero divide

This allows different CostOS modules to register errors without implementing their own separate error-display systems.

Errors Table Interface

The Errors Table temporarily changes the Shell prompt:

shell.INPUT1 = "ErrorsTable -";

It then provides a dedicated command loop:

ErrorsTable - User# getErrors

------------Errors Table------------
Module [BIOS]:
  - Failed to load configuration

ErrorsTable - User# exit

OS - User#
Boot & System Flow Demo
Welcome to Cost OS cpp!
Enter A to enter to the system, enter DEL to enter into the BIOS settings
# a

OS - User
# mathMode

Math - User
# calc
Enter a num operation num2: 10 * 5
Result: 50

Math - User
# exit

OS - User
# errorsTable

ErrorsTable - User
# getErrors

------------Errors Table------------
Module [Math]:
  - Zero divide

ErrorsTable - User
# exit

OS - User
# _
Project File Structure
├── BIOS/
│   ├── BIOSCore.hpp
│   └── Config.conf
├── CostOSBoot/
│   ├── CostOsCore.hpp
│   └── OSBoot.cpp
├── modules/
│   ├── Errors_Table.cpp
│   ├── Errors_Table.hpp
│   ├── math_module.cpp
│   └── math_module.hpp
├── Shell/
│   ├── Shell.hpp
│   └── ShellCore.hpp
└── README.md
Architectural Insights

<details> <summary>Click to view how BIOS configuration parsing works</summary>

Type-Safe Config Visitor

The BIOS configuration system utilizes std::variant, member pointers, and std::visit to safely map values from BIOS/Config.conf to runtime variables.

using BoolPtr = bool BIOS::*;
using IntPtr = int BIOS::*;
using FieldPtr = std::variant<BoolPtr, IntPtr>;

std::visit([this, &valueStr](auto&& field_ptr) {
    using T = std::decay_t<decltype(field_ptr)>;

    if constexpr (std::is_same_v<T, BoolPtr>) {
        this->*field_ptr = (valueStr == "true");
    }

    // Handles numerical assignments as well
}, it->second);

</details>

<details> <summary>Click to view how Shell command dispatching works</summary>

Member Function Pointer Map

Instead of using a large chain of if-else statements, the Shell stores commands in an std::unordered_map containing member function pointers.

std::unordered_map<std::string, void (Shell::*)(const Args&)> commands;

auto it = commands.find(cmd);

if (it != commands.end()) {
    (this->*(it->second))(args);
}

This allows new Shell commands to be registered without modifying a large conditional command dispatcher.

</details>

<details> <summary>Click to view how the Math Subshell interfaces with the OS</summary>

Procedural Function Mapping & Prompt Lifecycle

The mathInterface receives a reference to the active Shell object and temporarily modifies the Shell prompt.

using command = void(*)(void);

shell.INPUT1 = "Math -";

static const std::unordered_map<std::string, command> mathCommands = {
    {"calc", &calcCin},
    {"sum", &reduceCin},
    {"avg", &averageCin}
};

The Math environment can execute registered functions and handle exceptions:

if (it != mathCommands.end()) {
    try {
        it->second();
    }
    catch (std::exception& err) {
        std::cout << "Error: " << err.what() << std::endl;
    }
}

</details>

<details> <summary>Click to view how the Errors Table works</summary>

Centralized Module Error Storage

The Errors Table stores errors by module:

std::unordered_map<std::string, std::vector<std::string>> errors;

Errors are registered through:

void addError(const std::string& module, const std::string& err) {
    errors[module].push_back(err);
}

The table can then iterate through every registered module and its errors:

for (const auto& [module, error_list] : errors) {
    std::cout << "Module [" << module << "]:\n";

    for (const auto& err : error_list) {
        std::cout << "  - " << err << "\n";
    }
}

The errorsTableInterface uses the existing Shell object to provide a dedicated runtime environment and restores the OS - prompt when the user exits.

</details>

Compilation & Build
Prerequisites
C++17 or newer compiler
GCC or MSVC
Windows environment
<conio.h> support for the native console boot interface
Clone the Repository
git clone https://github.com/spevakovkos-ship-it/CostOS.git
cd CostOS
Compile with GCC
g++ -std=c++17 CostOSBoot/OSBoot.cpp modules/math_module.cpp modules/Errors_Table.cpp -o CostOS.exe
Run
./CostOS.exe
Current Architecture
                         CostOS
                            |
             +--------------+--------------+
             |              |              |
            BIOS           Shell       Modules
             |              |              |
       Config.conf          |       +------+------+
                            |       |             |
                         Commands   Math       Errors Table
                            |
                       Access Levels
                    USER / ROOT / SYSTEM

CostOS is an ongoing hobby project focused on experimenting with C++, modular architecture, CLI systems, configuration management, and system-like software design.