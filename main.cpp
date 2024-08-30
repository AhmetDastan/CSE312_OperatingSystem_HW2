#include <iostream> 
#include <cstdint>
#include "CdRom.hpp"

using namespace std;
 

int main() { 
    cout << "Hello, World!" << endl; 

    int sizeOfBlock = 4;
    string nameOfFile = "data.dat";
    string rootOfCd = "/root";
    CdRom fileSystem(sizeOfBlock, nameOfFile, rootOfCd);
    cout << "ananas \n";
    return 0;
}
    