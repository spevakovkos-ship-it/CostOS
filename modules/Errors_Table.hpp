#pragma once
#include <string>
#include <vector>
#include <unordered_map>

class Shell;

void addError(const std::string& module,const std::string& err);
void errorsTableInterface(Shell&shell);
