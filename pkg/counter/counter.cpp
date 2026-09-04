#include "counter.hpp"
#include <iostream>
#include <thread>
#include <chrono>
void counter(const Args& args) {
    if (counterDownloaded) {
    using namespace std;
    if (args.empty() || args.size() < 3) return;
    string range = args[0];
    string rawTime = args[1];
    int time = stoi(rawTime);
    string rawTimeLit = args[2];
    char timeLit = rawTimeLit.at(0);

    size_t colonPos = range.find('-');
    int max,min;
    if (colonPos != string::npos) {
        string rawMin = range.substr(0,colonPos);
        string rawMax = range.substr(colonPos + 1);
        min = stoi(rawMin);
        max = stoi(rawMax);

    } 

    for (int i = min;i < max;++i) {
        cout << "Counter:" << i+1 << endl; 
        switch (timeLit)
        {
        case 's':
            this_thread::sleep_for(chrono::seconds(time));
        break;
        case 'm':
            this_thread::sleep_for(chrono::milliseconds(time));
        break;
        case 'n':
            this_thread::sleep_for(chrono::nanoseconds(time));
        break;
        }
    }
}
}   