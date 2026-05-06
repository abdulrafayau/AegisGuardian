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
    std::string identifier;

public:
    FileBlock(std::string name, uintmax_t size, std::string fHash, std::string pHash, std::string id = "Unknown") 
        : filename(name), fileSize(size), fileHash(fHash), prevBlockHash(pHash), identifier(id) {
        timestamp = std::time(nullptr);
        generateOwnHash();
    }

    FileBlock() : fileSize(0), timestamp(0), identifier("Unknown") {}

    void generateOwnHash() {
        std::string data = filename + std::to_string(fileSize) + fileHash + prevBlockHash + std::to_string(timestamp) + identifier;
        ownBlockHash = Hasher::calculateStringHash(data);
    }

    std::string getFilename() const { return filename; }
    std::string getFileHash() const { return fileHash; }
    std::string getOwnBlockHash() const { return ownBlockHash; }
    std::string getPrevBlockHash() const { return prevBlockHash; }
    uintmax_t getFileSize() const { return fileSize; }
    time_t getTimestamp() const { return timestamp; }
    std::string getIdentifier() const { return identifier; }

    void setFilename(std::string val) { filename = val; }
    void setFileSize(uintmax_t val) { fileSize = val; }
    void setFileHash(std::string val) { fileHash = val; }
    void setPrevBlockHash(std::string val) { prevBlockHash = val; }
    void setOwnBlockHash(std::string val) { ownBlockHash = val; }
    void setTimestamp(time_t val) { timestamp = val; }
    void setIdentifier(std::string val) { identifier = val; }
};

#endif
