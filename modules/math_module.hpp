#pragma once
#include <string>
#include <iostream>
#include <vector>
class Shell;

void calcCin();
int calc(int firstNumber,int secondNumber,char operation); 


void mathInterface(Shell& shell); 

void averageCin();
double average(std::vector<int>&vec);

void reduceCin();
int reduce(std::vector<int>&vec);

void mathClear();

void generateRandomIntCin();
int generateRandomInt(int min,int max);