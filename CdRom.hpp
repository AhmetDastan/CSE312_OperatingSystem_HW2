#ifndef CDROM_H
#define CDROM_H

#include <iostream>
#include <cstdint>
#include <vector>


using namespace std;

struct Data{
    unsigned int dataSize;
    bool dirtyData;
};

struct DataBlocks{
    vector<Data> data;
};
 
struct DirectoryEntry{  // 1 + 1 + 8 + 8 + 14 + 2 + 1 + 15 = 50 bytes
    unsigned char dirEntryLength; // 1 byte unsigned integer
    unsigned char dirExtendSize; // 1 byte unsigned integer
    uint64_t locationOfFile; // 8 bytes to took location of file
    uint64_t fileSize; // 8 bytes unsigned integer
    char dateAndTime[14]; // m1/m2/d1/d2/y1/y2/y3/y4/h1/h2/min1/min2/s1/s2
    bool flags[2]; // 2 bytes for flags // first byte for dir is 1 or file is 0, second byte for extended attribute
    unsigned char L; // 1 byte unsigned integer for Length of file name
    char fileName[15]; // 15 bytes for file name // baseName(8) + (.) + extension(3) + (;) + var(2)
}; 

struct Directory{
    vector<DirectoryEntry> entries;
    Directory *parent;
    Directory *current;
};
 
struct SuperBlock{
    uint8_t superBlockSize = 54; // static 54 bytes
    uint16_t blockSize; // 2 bytes unsigned integer for block size -- it will took 64kb (my max block size)
    uint16_t totalBlocks; // 2 bytes unsigned integer for total blocks -- max total block is 16mb / 1kb = 16kb blocks
    Directory rootDirectory; // 50 bytes
};


class CdRom
{
public:
    CdRom(int sizeOfBlock, string nameOfFile, string rootOfCd);
    ~CdRom();

    void createCdRom();
    void createFile(string fileName, string fileContent);
    

private:
    SuperBlock superBlock;
    DataBlocks dataBlocks;

    void initiliseSuperBlock(int sizeOfBlock);
    void initiliseDirectory(Directory *directory);
    void initiliseDirectoryEntry(DirectoryEntry *directoryEntry, string fileName, string fileContent);
};



#endif
