#include <iostream> 
#include <cstdint>
#include "CdRom.hpp"

using namespace std;
 

int main() { 
    cout << "Hello, World!" << endl; 

    int sizeOfBlock = 4;
    string nameOfFile = "mySystem.dat";
    string rootOfCd = "/ysa/start";
    CdRom fileSystem(sizeOfBlock, nameOfFile, rootOfCd);
    
    cout << "ananas \n";
    return 0;
}
    