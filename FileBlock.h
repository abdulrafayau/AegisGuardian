#ifndef FILEBLOCK_H
#define FILEBLOCK_H

#include <string>
#include <ctime>
#include "Hasher.h"

class FileBlock {
private:
    std::string filename;
    uintmax_t fileSize;
    std::string fileHash;
    std::string prevBlockHash;
    std::string ownBlockHash;
    time_t timestamp;

public:
    FileBlock(std::string name, uintmax_t size, std::string fHash, std::string pHash) 
        : filename(name), fileSize(size), fileHash(fHash), prevBlockHash(pHash) {
        timestamp = std::time(nullptr);
        generateOwnHash();
    }

    // Default constructor for serialization
    FileBlock() : fileSize(0), timestamp(0) {}

    void generateOwnHash() {
        // The block hash is a combination of everything in the block
        // This creates the "link" in the blockchain
        std::string data = filename + std::to_string(fileSize) + fileHash + prevBlockHash;
        ownBlockHash = Hasher::calculateStringHash(data);
    }

    // Getters
    std::string getFilename() const { return filename; }
    std::string getFileHash() const { return fileHash; }
    std::string getOwnBlockHash() const { return ownBlockHash; }
    std::string getPrevBlockHash() const { return prevBlockHash; }
    uintmax_t getFileSize() const { return fileSize; }

    // For file I/O
    void setFilename(std::string val) { filename = val; }
    void setFileSize(uintmax_t val) { fileSize = val; }
    void setFileHash(std::string val) { fileHash = val; }
    void setPrevBlockHash(std::string val) { prevBlockHash = val; }
    void setOwnBlockHash(std::string val) { ownBlockHash = val; }
};

#endif
