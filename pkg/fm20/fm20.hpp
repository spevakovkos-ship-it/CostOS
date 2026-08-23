#pragma once
#include <vector>
#include <string>
inline bool fm20Downloaded = false;

void wc_fn(const std::vector<std::string>& args);
void find_fn(const std::vector<std::string>& args);
void grep_fn(const std::vector<std::string>& args);
void reverse_fn(const std::vector<std::string>& args);
void tail_fn(const std::vector<std::string>& args);
void head_fn(const std::vector<std::string>& args); 

void fm20(const std::vector<std::string>& args);
