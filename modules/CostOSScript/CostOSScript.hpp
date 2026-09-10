#pragma once
#include <vector>
#include <string>

class Shell;

void COSCreateScript(std::vector<std::string>&args,Shell&shell);
void COSExecuteScript(std::vector<std::string>&args,Shell&shell);
void COSWriteToScript(std::vector<std::string>&args,Shell&shell);
void COSRemoveLineInScript(std::vector<std::string>&args,Shell&shell);
void COSReadScript(std::vector<std::string>&args,Shell&shell);
void COSHelp(std::vector<std::string>&args,Shell&shell);
void COSRemoveScript(std::vector<std::string>&args,Shell&shell);


void CostOSScript(Shell&shell);