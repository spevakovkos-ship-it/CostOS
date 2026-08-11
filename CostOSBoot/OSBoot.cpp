#include "CostOsCore.hpp"



int main() {
    BIOS bios;
    Shell s(bios);
    CostOS os(bios,s);
    os.Boot();
    return 0;
}