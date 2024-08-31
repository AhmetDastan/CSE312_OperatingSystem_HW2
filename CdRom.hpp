#ifndef CDROM_H
#define CDROM_H

#include "FileManager.hpp"
#include <iostream>
#include <cstdint>
#include <fstream>
#include <ctime>
#include <vector>
#include <filesystem>
#include <string.h>
#include <string>
 

using namespace std;


struct DataBlocks{
    vector<bool> dirtyData;
    vector<string> dataName;
    int blockAmount; 
};
 
struct DirectoryEntry{  // 1 + 1 + 8 + 8 + 14 + 2 + 1 + 15 = 50 bytes    -1 L =>> now 49bytes
    unsigned char dirEntryLength; // 1 byte unsigned integer
    unsigned char dirExtendSize; // 1 byte unsigned integer
    uint64_t locationOfFile; // 8 bytes to took location of file
    uint64_t fileSize; // 8 bytes unsigned integer
    char dateAndTime[14]; // m1/m2/d1/d2/y1/y2/y3/y4/h1/h2/min1/min2/s1/s2
    bool flags[2]; // 2 bytes for flags // first byte for dir is 1 or file is 0, second byte for extended attribute 
    char fileName[15]; // 15 bytes for file name // baseName(8) + (.) + extension(3) + (;) + var(2)
}; 

struct Directory{
    vector<DirectoryEntry> entries;
    int amountOfEntries;
};

 
struct SuperBlock{
    uint16_t superBlockByteSize = 8; // 2 byte static 54 bytes
    uint16_t blockSize; // 2 bytes unsigned integer for block size -- it will took 64kb (my max block size)
    uint16_t totalBlocks; // 2 bytes unsigned integer for total blocks -- max total block is 16mb / 1kb = 16kb blocks
    uint16_t rootDirectoryBlockLocation = 1; // 2 bytes to point of location root directory
};


class CdRom
{
public:
    CdRom(int sizeOfBlock, string nameOfFile, string rootOfCd); 
  
    
    FileManager fileManager;

private:
    string nameOfFile, rootOfCd;
    int sizeOfBlock;
    SuperBlock superBlock;
    DataBlocks dataBlocks; 
    
    void initiliseSuperBlock(int sizeOfBlock); 
    void initiliseDirectoryEntry(DirectoryEntry *directoryEntry, string fileName, string fileContent);
    void initiliseRootDirectory();

    void writeFolderToCd(string path);

    int howManyBlocksNeeded(unsigned int size);
    int nextProperPositionOfDataBlocks(int needOfBlock);
     
    // for testing function
    void printDataBlocksDirtyDatas();
};



#endif
