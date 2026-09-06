#pragma once
#include <vector>
#include <string>

class Shell;

void COSCreateScript(std::vector<std::string>&args,Shell&shell);
void COSExecuteScript(std::vector<std::string>&args,Shell&shell);

void CostOSScript(Shell&shell);