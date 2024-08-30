#include "CdRom.hpp" 


using namespace std;

CdRom::CdRom(int blockSize, string fileName, string rootOfCd) {
    this->nameOfFile = fileName;
    this->rootOfCd = rootOfCd;
    this->sizeOfBlock = blockSize * 1024;
    fileManager = FileManager(nameOfFile);

    initiliseSuperBlock(sizeOfBlock);
    cout << "CdRom constructor" << endl;
}


void CdRom::initiliseSuperBlock(int sizeOfBlock) {
    
    superBlock.blockSize = sizeOfBlock;
    superBlock.totalBlocks = 16 * 8 * 1024 * 1024 / sizeOfBlock; // 16mb / sizeOfBlock
    cout << "CdRom initiliseSuperBlock " << superBlock.blockSize << "ve " << superBlock.totalBlocks <<endl;
    initiliseRootDirectory();
}

void CdRom::initiliseRootDirectory() {
    superBlock.rootDirectory.parent = nullptr;
    superBlock.rootDirectory.current = nullptr;

    filesystem::path dirPath("." + rootOfCd);

    for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
        DirectoryEntry de = {};
        std::string fileName = entry.path().filename().string();
        std::string fileExt = entry.path().extension().string();
        std::string baseName = fileName.substr(0, fileName.find_last_of('.'));
        cout<< "File name: " << fileName << " File extension: " << fileExt << " Base name: " << baseName << endl;
        de.dirEntryLength = 50;
        de.dirExtendSize = 0;
        de.locationOfFile = 0;
        de.fileSize = 0;
        de.dateAndTime[14] = {};
        de.flags[0] = entry.is_directory() ? 1 : 0;
        de.flags[1] = 0; // No extended attributes
        de.L = 0;
        de.fileName[15] = {};
        if(!de.flags[0])
            cout<< " size of file " << file_size(entry.path()) <<endl;


        //superBlock.rootDirectory.entries.push_back(de);
    }

    cout << "CdRom initiliseRootDirectory" << endl;
}

void CdRom::initiliseDirectory(Directory *directory){ 
    directory->parent = nullptr;
    directory->current = nullptr;

    //filesystem::directory_entry entry;
    
    filesystem::path dirPath("." + rootOfCd);

    for (const auto& entry : std::filesystem::directory_iterator(dirPath)) { 
        std::string fileName = entry.path().filename().string();
        std::string fileExt = entry.path().extension().string();
        std::string baseName = fileName.substr(0, fileName.find_last_of('.'));
        cout<< "File name: " << fileName << " File extension: " << fileExt << " Base name: " << baseName << endl;
    }

    

    
    cout << "CdRom initiliseDirectory" << endl;
}

void CdRom::initiliseDirectoryEntry(DirectoryEntry *directoryEntry, string fileName, string fileContent){ // 50 byte decrotry entry initilise

}