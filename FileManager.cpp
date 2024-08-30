#include "FileManager.hpp"

using namespace std;

FileManager::FileManager(string fileName) {
    this->fileName = fileName; 
    this->indexAmount = indexAmount;
    fileSizeAsBit = 0;//134217728;
    initiliseFile(fileName, fileSizeAsBit);
    cout << "File constructor" << endl;
}
 

void FileManager::initiliseFile(string fileName, unsigned int fileSize){  
    /*ofstream file(fileName, ios::binary); 
    int counter = 0;
    while(counter < fileSize){
        file << 0;
        ++counter;
    }
 
    file.close(); */
}

void FileManager::write(const char* data, unsigned int size){ 
    ofstream file(fileName, std::ios::binary | std::ios::app); // Append mode
    if (file.is_open()) {
        file.write(data, size); // convert binary formation
        file.close();
    }
}

int FileManager::read(char* data, unsigned int size){      // index include offset
    ifstream file(fileName, std::ios::binary);
    
    char value;
    while (file.read(reinterpret_cast<char*>(&value), sizeof(value))) {
        std::cout << "Read value: " << value << std::endl;
    } 

    file.close(); 
    return value;
}

