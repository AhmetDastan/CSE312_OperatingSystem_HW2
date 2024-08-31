#include "CdRom.hpp" 


using namespace std;

CdRom::CdRom(int blockSize, string fileName, string path) {
    this->nameOfFile = fileName;
    this->rootOfCd = path;
    this->sizeOfBlock = blockSize * 1024;

    dataBlocks.blockAmount = 16 * 8 * 1024 * 1024 / sizeOfBlock; // 16mb / sizeOfBlock 
    dataBlocks.dirtyData.resize(dataBlocks.blockAmount, false); // 16mb / sizeOfBlock
    dataBlocks.dataName.resize(dataBlocks.blockAmount, " ");

    fileManager = FileManager(nameOfFile);

    initiliseSuperBlock(sizeOfBlock);
    writeFolderToCd(rootOfCd);
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
    dataBlocks.dataName[0] = "superBlock"; 


    initiliseRootDirectory();
}

void CdRom::initiliseRootDirectory() { 

    dataBlocks.dirtyData[1] = true; // root directory is dirty 
    dataBlocks.dataName[1] = "rootDirectory";
  
    unsigned int rootDirSize = 0;
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
        
        rootDirSize += de.dirEntryLength;
        
        fileManager.write((char*)&de, sizeof(de));

        dataBlocks.dataName[de.locationOfFile] = de.fileName; 
    }

    // for . and .. create directory entry and assign to root directory
    time_t currentTime = std::time(nullptr);
    time_t cftime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    DirectoryEntry currentDir = {};
    currentDir.dirEntryLength = 47;
    currentDir.dirExtendSize = 0;
    currentDir.locationOfFile = 1;  // its root directory
    currentDir.fileSize = 0;
    currentDir.flags[0] = 1;
    currentDir.flags[1] = 0;
    currentDir.dateAndTime[14] = {};
    strftime(currentDir.dateAndTime, sizeof(currentDir.dateAndTime), "%d%m%Y%H%M%S", localtime(&currentTime));
    currentDir.fileName[15] = {};
    copy(".", " ", currentDir.fileName); 

    DirectoryEntry currentDir2 = {};
    currentDir2.dirEntryLength = 47;
    currentDir2.dirExtendSize = 0;
    currentDir2.locationOfFile = 1;  // its root directory
    currentDir2.fileSize = 0;
    currentDir2.flags[0] = 1;
    currentDir2.flags[1] = 0;
    currentDir2.dateAndTime[14] = {};
    strftime(currentDir2.dateAndTime, sizeof(currentDir2.dateAndTime), "%d%m%Y%H%M%S", localtime(&currentTime));
    currentDir2.fileName[15] = {};
    copy("..", " ", currentDir2.fileName); 


    rootDirSize += currentDir.dirEntryLength + currentDir2.dirEntryLength;
    fileManager.write((char*)&currentDir, sizeof(currentDir));
    fileManager.write((char*)&currentDir2, sizeof(currentDir2)); 

    // fill remain of block with 0
    char *buffer = new char[sizeOfBlock - rootDirSize];
    memset(buffer, 0, sizeOfBlock - rootDirSize);
    fileManager.write(buffer, sizeOfBlock - rootDirSize);
    delete[] buffer;   
}
 

void CdRom::initiliseDirectoryEntry(DirectoryEntry *directoryEntry, string fileName, string fileContent){ // 50 byte decrotry entry initilise
    
}

void CdRom::writeFolderToCd(string path){
    cout << " write folder to cd \n";

    // read root directory

    path = "." + path;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {  //recursive_directory_iterator
        if (entry.is_directory()) {
            std::cout << "Directory: " << entry.path() << std::endl;
        } else if (entry.is_regular_file()) {
            std::cout << "File: " << entry.path() << std::endl;
        }
    }
    return;
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
                        dataBlocks.dataName[index + k] = "X";
                    }
                    return index;
                }
            }
        }
    }
    dataBlocks.dirtyData[index] = true; // mark as dirty
    return index;
}

void CdRom::printDataBlocksDirtyDatas(){ 
    for(int i = 0; i < 10; ++i){
        cout << dataBlocks.dirtyData[i] << " ";
        cout << dataBlocks.dataName[i] << endl;
    }
    cout << endl;
}