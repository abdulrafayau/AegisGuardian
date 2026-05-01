#ifndef GUARDIAN_H
#define GUARDIAN_H

#include <vector>
#include <fstream>
#include <filesystem>
#include <random>
#include "FileBlock.h"
#include "Utils.h"

namespace fs = std::filesystem;

class Guardian {
private:
    std::vector<FileBlock> chain;
    const std::string LEDGER_NAME = "ledger.guardian";
    const std::string BACKUP_DIR = ".backups";
    const std::string DOCS_DIR = "Department_Docs";

    std::string generateOTP() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(100000, 999999);
        return std::to_string(dis(gen));
    }

public:
    void initialize() {
        if (!fs::exists(DOCS_DIR)) fs::create_directory(DOCS_DIR);
        if (!fs::exists(BACKUP_DIR)) fs::create_directory(BACKUP_DIR);

        chain.clear();
        std::string lastHash = "00000000";

        for (const auto& entry : fs::directory_iterator(DOCS_DIR)) {
            if (fs::is_regular_file(entry)) {
                std::string filename = entry.path().filename().string();
                uintmax_t size = fs::file_size(entry);
                std::string fHash = Hasher::calculateFileHash(entry.path().string());

                fs::copy_file(entry.path(), BACKUP_DIR + "/" + filename, fs::copy_options::overwrite_existing);

                FileBlock block(filename, size, fHash, lastHash);
                lastHash = block.getOwnBlockHash();
                chain.push_back(block);
            }
        }
        saveLedger();
    }

    void saveLedger() {
        std::ofstream file(LEDGER_NAME);
        for (const auto& b : chain) {
            file << b.getFilename() << "|" << b.getFileSize() << "|" 
                 << b.getFileHash() << "|" << b.getPrevBlockHash() << "|" 
                 << b.getOwnBlockHash() << "\n";
        }
    }

    bool loadLedger() {
        if (!Utils::fileExists(LEDGER_NAME)) return false;
        chain.clear();
        std::ifstream file(LEDGER_NAME);
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string name, sizeStr, fHash, pHash, oHash;
            std::getline(ss, name, '|');
            std::getline(ss, sizeStr, '|');
            std::getline(ss, fHash, '|');
            std::getline(ss, pHash, '|');
            std::getline(ss, oHash, '|');

            if (name.empty()) continue;
            FileBlock b;
            b.setFilename(name);
            b.setFileSize(std::stoull(sizeStr));
            b.setFileHash(fHash);
            b.setPrevBlockHash(pHash);
            b.setOwnBlockHash(oHash);
            chain.push_back(b);
        }
        return true;
    }

    void viewFile(const std::string& filename) {
        std::string filePath = DOCS_DIR + "/" + filename;
        std::ifstream file(filePath);
        if (file.is_open()) {
            Utils::printHeader("VIEWING: " + filename);
            Utils::setColor(Utils::WHITE);
            std::string line;
            while (std::getline(file, line)) {
                std::cout << "  " << line << "\n";
            }
            Utils::reset();
            Utils::printSeparator();
        } else {
            Utils::printError("Could not open file.");
        }
    }

    void authorizedUpdate(const std::string& filename, const std::string& newData) {
        std::string otp = generateOTP();
        Utils::setColor(Utils::YELLOW);
        std::cout << "\n[AUTH] AUTHORIZATION REQUIRED FOR UPDATE\n";
        std::cout << "OTP: " << otp << "\n";
        Utils::reset();

        std::string userInput;
        std::cout << "Enter OTP to confirm: ";
        std::cin >> userInput;

        if (userInput == otp) {
            std::string filePath = DOCS_DIR + "/" + filename;
            std::ofstream file(filePath, std::ios::app); 
            if (file.is_open()) {
                file << newData; // Appending the multi-line data
                file.close();
                initialize(); 
                Utils::printSuccess("Bulk update successful and blockchain re-sealed.");
            }
        } else {
            Utils::printError("INVALID OTP. UPDATE ABORTED.");
        }
    }

    void addNewFile(const std::string& filename, const std::string& content) {
        std::string otp = generateOTP();
        Utils::setColor(Utils::YELLOW);
        std::cout << "\n[AUTH] AUTHORIZATION REQUIRED FOR NEW FILE\n";
        std::cout << "OTP: " << otp << "\n";
        Utils::reset();

        std::string userInput;
        std::cout << "Enter OTP to confirm: ";
        std::cin >> userInput;

        if (userInput == otp) {
            std::string filePath = DOCS_DIR + "/" + filename;
            std::ofstream file(filePath);
            if (file.is_open()) {
                file << content;
                file.close();
                initialize();
                Utils::printSuccess("Bulk document added and sealed.");
            }
        } else {
            Utils::printError("INVALID OTP. CREATION ABORTED.");
        }
    }

    void restoreAll() {
        Utils::printHeader("RESTORING FROM BACKUP");
        for (const auto& block : chain) {
            std::string originalPath = DOCS_DIR + "/" + block.getFilename();
            std::string backupPath = BACKUP_DIR + "/" + block.getFilename();
            if (fs::exists(backupPath)) {
                fs::copy_file(backupPath, originalPath, fs::copy_options::overwrite_existing);
                std::cout << "[RESTORED] " << block.getFilename() << "\n";
            }
        }
        Utils::printSuccess("System files restored to original state.");
    }

    void verify() {
        if (!loadLedger()) {
            Utils::printError("No ledger found. Initialize first.");
            return;
        }

        Utils::printHeader("DEPARTMENT DOCUMENTS INTEGRITY REPORT");
        bool breach = false;
        std::string expectedPrev = "00000000";

        for (const auto& block : chain) {
            std::string filePath = DOCS_DIR + "/" + block.getFilename();
            
            if (block.getPrevBlockHash() != expectedPrev) {
                Utils::setColor(Utils::RED);
                std::cout << "[BROKEN CHAIN] " << block.getFilename() << "\n";
                breach = true;
            }
            expectedPrev = block.getOwnBlockHash();

            if (!Utils::fileExists(filePath)) {
                Utils::setColor(Utils::RED);
                std::cout << "[MISSING]  " << block.getFilename() << "\n";
                breach = true;
            } else {
                std::string currentHash = Hasher::calculateFileHash(filePath);
                if (currentHash != block.getFileHash()) {
                    Utils::setColor(Utils::RED);
                    std::cout << "[TAMPERED] " << block.getFilename() << "\n";
                    breach = true;
                } else {
                    Utils::setColor(Utils::GREEN);
                    std::cout << "[SECURE]   " << block.getFilename() << "\n";
                }
            }
            Utils::reset();
        }

        if (breach) {
            Utils::setColor(Utils::RED);
            std::cout << "\n!!! BREACH DETECTED !!!\n";
            Utils::reset();
        } else {
            Utils::printSuccess("All department documents are secure.");
        }
    }

    std::vector<std::string> getFileList() {
        std::vector<std::string> files;
        for (const auto& b : chain) files.push_back(b.getFilename());
        return files;
    }
};

#endif
