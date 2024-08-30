#include "CdRom.hpp"


using namespace std;

CdRom::CdRom(int sizeOfBlock, string nameOfFile, string rootOfCd) {
    cout << "CdRom constructor" << endl;
}

CdRom::~CdRom() {
    cout << "CdRom destructor" << endl;
}

void CdRom::initiliseSuperBlock(int sizeOfBlock) {
    superBlock.blockSize = sizeOfBlock;
    superBlock.totalBlocks = 16 * 1024 / sizeOfBlock; // 16mb / sizeOfBlock
    initiliseDirectory(&superBlock.rootDirectory);
    cout << "CdRom initiliseSuperBlock" << endl;
}

void CdRom::initiliseDirectory(Directory *directory){ 
    directory->parent = nullptr;
    directory->current = nullptr;
    cout << "CdRom initiliseDirectory" << endl;
}

void CdRom::initiliseDirectoryEntry(DirectoryEntry *directoryEntry, string fileName, string fileContent){ // 50 byte decrotry entry initilise

}