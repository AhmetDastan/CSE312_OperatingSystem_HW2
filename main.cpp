#include <iostream> 
#include <cstdint>
#include "CdRom.hpp"
#include "FileManager.hpp"

using namespace std;
 
void operationDir(string path);

int main() { 
    cout << "Hello, World!" << endl; 

    int sizeOfBlock = 4;
    string nameOfFile = "mySystem.dat";
    string rootOfCd = "/ysa/start";

    CdRom fileSystem(sizeOfBlock, nameOfFile, rootOfCd); 
    cout << "ananas \n";

    operationDir("/ysa/start");
    cout << " bu is bukadar \n";
    return 0;
}
    


// there are 3 operation

//operation 1 is list all contents of the root directory. Go superblock and then find root directory point. Then list shole entries are there defined
void operationDir(string path){
    // open .dat file and read superblock
    // find root directory location
    // read root directory
    // print all entries

    ifstream file("mySystem.dat", std::ios::binary);

    if (!file) {
        std::cerr << "Dosya açma hatası: " << "mySystem.dat" << std::endl;
        return;
    }

    SuperBlock superBlock;
    file.read((char*)&superBlock, sizeof(superBlock));
    
    file.seekg(superBlock.rootDirectoryBlockLocation * superBlock.blockSize);
    DirectoryEntry rootEntry;

    for(int i = 0; i < 5; ++i){
        file.read((char*)&rootEntry, sizeof(rootEntry));
        cout << "pekki ya simdi " << rootEntry.fileName << endl;        
    }
 
    /*for(int i = 0; i < rootDirectory.amountOfEntries; ++i){
        cout << rootDirectory.entries[i].fileName << endl;
    }*/

    file.close();

    return;
}

// operation 2 is block size, block count, file and dir count. And addition print occupied blocks with their file names.
void operationDumpe2fs(){

}

// operation 3 is copy whole directory to another linux file
void operationRead(string path, string fileName){

}