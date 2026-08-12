# CostOS cpp

A hobbyist CLI operating system emulator written in C++.

CostOS is a modular console-based system with a virtual BIOS, interactive Shell, Math Module, and centralized Errors Table.

## Features

- Virtual BIOS configuration
- Interactive bootloader
- BIOS setup accessible through `DEL`
- Persistent configuration file
- CLI Shell with command parsing
- User access levels: `USER`, `ROOT`, `SYSTEM`
- ANSI color output
- Dedicated Math subshell
- Centralized Errors Table
- Modular project structure
- C++17

## Core Shell Commands

| Command | Arguments | Description |
|---|---|---|
| `clear` | — | Clears the terminal screen. |
| `print` | `<text1> [text2] ...` | Prints one or more arguments. |
| `colorPrint` | `<Color> <text> ...` | Prints text using ANSI colors. |
| `mathMode` | — | Opens the Math subshell. |
| `errorsTable` | — | Opens the Errors Table. |

## Math Subshell

The Math environment is a separate command interface launched with `mathMode`.

| Command | Arguments | Description |
|---|---|---|
| `calc` | — | Performs basic mathematical calculations. |
| `getRandNum` | — | Generates a random number within a specified range. |
| `sum` | — | Calculates the sum of entered integers. |
| `avg` / `average` | — | Calculates the average of entered integers. |
| `clear` | — | Clears the Math terminal. |
| `exit` / `q` / `quit` | — | Returns to the main OS Shell. |

Example:

```text
OS - User
# mathMode

Math - User
# calc
Enter a num operation num2: 10 * 5
Result: 50

Math - User
# exit

OS - User
#
```

## Errors Table

CostOS contains a centralized Errors Table for storing runtime errors from different modules.

Errors are stored by module using:

```cpp
std::unordered_map<std::string, std::vector<std::string>> errors;
```

An error can be added with:

```cpp
addError("BIOS", "Failed to load configuration");
addError("Shell", "Unknown command");
addError("Math", "Zero divide");
```

### Errors Table Commands

| Command | Description |
|---|---|
| `getErrors` | Displays all registered errors. |
| `exit` | Exits the Errors Table. |
| `q` | Exits the Errors Table. |
| `quit` | Exits the Errors Table. |

Example:

```text
ErrorsTable - User
# getErrors

------------Errors Table------------
Module [BIOS]:
  - Failed to load configuration

Module [Shell]:
  - Unknown command

Module [Math]:
  - Zero divide

ErrorsTable - User
# exit

OS - User
#
```

The Errors Table uses the existing Shell instance and temporarily changes its prompt:

```cpp
void errorsTableInterface(Shell& shell) {
    shell.INPUT1 = "ErrorsTable -";

    // Errors Table command loop
}
```

When the user exits the interface, the prompt is restored to `OS -`.

## BIOS

The BIOS is responsible for CostOS configuration.

The configuration is stored in:

```text
BIOS/Config.conf
```

The BIOS can control system options such as:

- Math Module availability
- Error logging
- Other system configuration values

The configuration system uses `std::variant`, member pointers, and `std::visit` to map configuration values to BIOS fields.

Example:

```cpp
using BoolPtr = bool BIOS::*;
using IntPtr = int BIOS::*;
using FieldPtr = std::variant<BoolPtr, IntPtr>;

std::visit([this, &valueStr](auto&& field_ptr) {
    using T = std::decay_t<decltype(field_ptr)>;

    if constexpr (std::is_same_v<T, BoolPtr>) {
        this->*field_ptr = (valueStr == "true");
    }
}, it->second);
```

## Shell Command Dispatcher

Shell commands are stored in an `std::unordered_map` using member function pointers.

```cpp
std::unordered_map<std::string, void (Shell::*)(const Args&)> commands;
```

Commands are found and executed dynamically:

```cpp
auto it = commands.find(cmd);

if (it != commands.end()) {
    (this->*(it->second))(args);
}
```

## Math Command Dispatcher

The Math Module uses function pointers to map command names to mathematical functions.

```cpp
using command = void(*)(void);

static const std::unordered_map<std::string, command> mathCommands = {
    {"calc", &calcCin},
    {"sum", &reduceCin},
    {"avg", &averageCin}
};
```

The Math environment temporarily changes the Shell prompt:

```cpp
shell.INPUT1 = "Math -";
```

When the Math environment is closed, the original OS prompt is restored.

## Boot Process

When CostOS starts, the bootloader waits for user input.

```text
Welcome to Cost OS cpp!
Enter A to enter to the system, enter DEL to enter into the BIOS settings

# a

OS - User
#
```

Pressing `DEL` opens the BIOS interface.

## Project Structure

```text
CostOS/
├── BIOS/
│   ├── BIOSCore.hpp
│   └── Config.conf
│
├── CostOSBoot/
│   ├── CostOsCore.hpp
│   └── OSBoot.cpp
│
├── modules/
│   ├── Errors_Table.cpp
│   ├── Errors_Table.hpp
│   ├── math_module.cpp
│   └── math_module.hpp
│
├── Shell/
│   ├── Shell.hpp
│   └── ShellCore.hpp
│
└── README.md
```

## Architecture

```text
                         CostOS
                            |
              +-------------+-------------+
              |             |             |
             BIOS          Shell        Modules
              |             |             |
        Config.conf         |       +-----+-----+
                            |       |           |
                         Commands   Math    Errors Table
                            |
                     USER / ROOT / SYSTEM
```

## Build

### Requirements

- C++17 or newer
- GCC or MSVC
- Windows
- `<conio.h>`

CostOS currently targets Windows because the bootloader uses `_getch()` and `_kbhit()`.

### Clone

```bash
git clone https://github.com/spevakovkos-ship-it/CostOS.git
cd CostOS
```

### Compile with GCC

```bash
g++ -std=c++17 CostOSBoot/OSBoot.cpp modules/math_module.cpp modules/Errors_Table.cpp -o CostOS.exe
```

### Run

```bash
./CostOS.exe
```

## Current Status

CostOS is an ongoing hobby project focused on C++ development, modular architecture, CLI systems, BIOS simulation, configuration management, command dispatching, and runtime error management.
