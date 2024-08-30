#include "CdRom.hpp" 


using namespace std;

CdRom::CdRom(int blockSize, string fileName, string rootOfCd) {
    this->nameOfFile = fileName;
    this->rootOfCd = rootOfCd;
    this->sizeOfBlock = blockSize * 1024;

    dataBlocks.blockAmount = 16 * 8 * 1024 * 1024 / sizeOfBlock; // 16mb / sizeOfBlock 
    dataBlocks.dirtyData.resize(dataBlocks.blockAmount, false); // 16mb / sizeOfBlock

    fileManager = FileManager(nameOfFile);

    initiliseSuperBlock(sizeOfBlock);
    cout << "CdRom constructor" << endl;
}


void CdRom::initiliseSuperBlock(int sizeOfBlock) {
    
    superBlock.blockSize = sizeOfBlock;
    superBlock.totalBlocks = 16 * 8 * 1024 * 1024 / sizeOfBlock; // 16mb / sizeOfBlock
    superBlock.superBlockByteSize = 8; // static 6 bytes = 48 bit
    superBlock.rootDirectoryBlockLocation = 1; // its static because superBlock dont be exceed first block of file
    
    fileManager.write((char*)&superBlock, sizeof(superBlock));

    // fill remain of block with 0
    char *buffer = new char[sizeOfBlock - sizeof(superBlock)];
    memset(buffer, 0, sizeOfBlock - sizeof(superBlock));
    fileManager.write(buffer, sizeOfBlock - sizeof(superBlock));
    delete[] buffer; 

    dataBlocks.dirtyData[0] = true; // superBlock is dirty  
  
    initiliseRootDirectory();
}

void CdRom::initiliseRootDirectory() { 

    dataBlocks.dirtyData[1] = true; // root directory is dirty 
    filesystem::path dirPath("." + rootOfCd);

    for (const auto& entry : filesystem::directory_iterator(dirPath)) {
        DirectoryEntry de = {};

        de.dirEntryLength = 47;
        de.dirExtendSize = 0; 

        // check dir flags to manage size or finding proper location
        de.flags[0] = entry.is_directory() ? 1 : 0;
        de.flags[1] = 0; // No extended attributes

        if(de.flags[0]){ // if directory 
            de.fileSize = 0;
            de.locationOfFile = nextProperPositionOfDataBlocks(1);
        }else{ 
            de.fileSize = file_size(entry.path());
            de.locationOfFile = nextProperPositionOfDataBlocks(howManyBlocksNeeded(de.fileSize / 8)); 
        }
        
        string fileName = entry.path().filename().string();
        
        time_t currentTime = std::time(nullptr);
        time_t cftime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        de.dateAndTime[14] = {};
        strftime(de.dateAndTime, sizeof(de.dateAndTime), "%d%m%Y%H%M%S", localtime(&currentTime));
        
        de.fileName[15] = {};
        copy(fileName.begin(), fileName.end(), de.fileName); 


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


int CdRom::howManyBlocksNeeded(unsigned int fileSize){
    int counter = 1; 
    while(fileSize > sizeOfBlock){
        fileSize -= sizeOfBlock;
        ++counter;
    } 
    return counter;
}

int CdRom::nextProperPositionOfDataBlocks(int needOfBlock){
    int index = 0;
    // find sequential block
    for(int i = 0; i < dataBlocks.blockAmount; ++i){
        if(!dataBlocks.dirtyData[i]){
            index = i;
            for(int j = 0; j < needOfBlock; ++j){ // find contogous data block
                ++i;
                if(dataBlocks.dirtyData[i]){
                    i = index;
                    break;
                }
                if((j+1) == needOfBlock){
                    for(int k = 0; k < needOfBlock; ++k){ // mark as dirty
                        dataBlocks.dirtyData[index + k] = true;
                    }
                    return index;
                }
            }
        }
    }
    dataBlocks.dirtyData[index] = true; // mark as dirty
    return index;
}