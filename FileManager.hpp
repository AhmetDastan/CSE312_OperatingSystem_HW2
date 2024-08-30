#ifndef FILEMANAGER_H
#define FILEMANAGER_H   

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class FileManager{
public:
    FileManager(){};
    FileManager(string fileName); // create a file with a name and file size

    void write(const char* data, unsigned int size);
    void read(char* data, unsigned int size, streampos offset); 
private:
    string fileName;
    int indexAmount;
    unsigned int fileSizeAsBit = 134217728; // 16mb static file size
    void initiliseFile(string file, unsigned int fileSize);
};  


#endif
