#ifndef GUARDIAN_H
#define GUARDIAN_H

#include <vector>
#include <fstream>
#include <filesystem>
#include <random>
#include <sstream>
#include "FileBlock.h"
#include "Utils.h"
#include "AuditLogger.h"

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


    std::string getSystemIdentifier() {
        const char* user = getenv("USERNAME");
        const char* comp = getenv("COMPUTERNAME");
        std::string id = (comp ? std::string(comp) : "UnknownHost");
        id += ":" + (user ? std::string(user) : "UnknownUser");
        return id;
    }

public:
    void initialize() {
        Utils::printLoadingBar("Initializing Shield");
        if (!fs::exists(DOCS_DIR)) fs::create_directory(DOCS_DIR);
        if (!fs::exists(BACKUP_DIR)) fs::create_directory(BACKUP_DIR);

        chain.clear();
        std::string lastHash = "00000000";
        std::string systemID = getSystemIdentifier();

        for (const auto& entry : fs::directory_iterator(DOCS_DIR)) {
            if (fs::is_regular_file(entry)) {
                std::string filename = entry.path().filename().string();
                uintmax_t size = fs::file_size(entry);
                std::string fHash = Hasher::calculateFileHash(entry.path().string());

                fs::copy_file(entry.path(), BACKUP_DIR + "/" + filename, fs::copy_options::overwrite_existing);

                FileBlock block(filename, size, fHash, lastHash, systemID);
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
                 << b.getOwnBlockHash() << "|" << b.getTimestamp() << "|" 
                 << b.getIdentifier() << "\n";
        }
    }

    bool loadLedger() {
        if (!Utils::fileExists(LEDGER_NAME)) return false;
        chain.clear();
        std::ifstream file(LEDGER_NAME);
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string name, sizeStr, fHash, pHash, oHash, timeStr, id;
            
            if (!std::getline(ss, name, '|') ||
                !std::getline(ss, sizeStr, '|') ||
                !std::getline(ss, fHash, '|') ||
                !std::getline(ss, pHash, '|') ||
                !std::getline(ss, oHash, '|') ||
                !std::getline(ss, timeStr, '|') ||
                !std::getline(ss, id, '|')) {
                continue; // Skip lines with missing fields
            }

            try {
                FileBlock b;
                b.setFilename(name);
                b.setFileSize(std::stoull(sizeStr));
                b.setFileHash(fHash);
                b.setPrevBlockHash(pHash);
                b.setOwnBlockHash(oHash);
                b.setTimestamp(std::stoll(timeStr));
                b.setIdentifier(id);
                chain.push_back(b);
            } catch (...) {
                continue; // Skip lines with invalid numeric data
            }
        }
        return true;
    }

    void viewFile(const std::string& filename) {
        std::string filePath = DOCS_DIR + "/" + filename;
        std::ifstream file(filePath);
        if (file.is_open()) {
            Utils::printHeader("VIEWING: " + filename);
            
            for (const auto& b : chain) {
                if (b.getFilename() == filename) {
                    time_t t = b.getTimestamp();
                    std::string timeStr = std::ctime(&t);
                    if (!timeStr.empty() && timeStr.back() == '\n') timeStr.pop_back();
                    std::cout << "  [BLOCKCHAIN METADATA]\n";
                    std::cout << "  Size: " << b.getFileSize() << " bytes\n";
                    std::cout << "  Hash: " << b.getFileHash() << "\n";
                    std::cout << "  Seal Date: " << timeStr << "\n";
                    std::cout << "  Author ID: " << b.getIdentifier() << "\n";
                    Utils::printSeparator();
                    break;
                }
            }

            std::string line;
            while (std::getline(file, line)) {
                std::cout << "  " << line << "\n";
            }
            Utils::printSeparator();
        } else {
            Utils::printError("Could not open file.");
        }
    }

    void addNewFile(const std::string& filename, const std::string& content) {
        std::string otp = generateOTP();
        std::cout << "\n[AUTH] AUTHORIZATION REQUIRED FOR NEW FILE\n";
        std::cout << "OTP: " << otp << "\n";

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
                Utils::printSuccess("Document added and sealed into blockchain.");
                AuditLogger::getInstance().log("USER", "Created document: " + filename);
            }
        } else {
            Utils::printError("INVALID OTP. CREATION ABORTED.");
        }
    }

    void authorizedRename(const std::string& oldName, const std::string& newName) {
        std::string otp = generateOTP();
        std::cout << "\n[AUTH] AUTHORIZATION REQUIRED FOR RENAME\n";
        std::cout << "OTP: " << otp << "\n";

        std::string userInput;
        std::cout << "Enter OTP to confirm: ";
        std::cin >> userInput;

        if (userInput == otp) {
            std::string oldPath = DOCS_DIR + "/" + oldName;
            std::string newPath = DOCS_DIR + "/" + newName;
            
            if (fs::exists(oldPath)) {
                fs::rename(oldPath, newPath);
                std::string oldBackup = BACKUP_DIR + "/" + oldName;
                std::string newBackup = BACKUP_DIR + "/" + newName;
                if (fs::exists(oldBackup)) fs::rename(oldBackup, newBackup);
                
                initialize();
                Utils::printSuccess("Document renamed and chain re-sealed.");
                AuditLogger::getInstance().log("USER", "Renamed " + oldName + " to " + newName);
            } else {
                Utils::printError("Original file does not exist.");
            }
        } else {
            Utils::printError("INVALID OTP. RENAME ABORTED.");
        }
    }

    void authorizedUpdate(const std::string& filename, const std::string& newData) {
        std::string otp = generateOTP();
        std::cout << "\n[AUTH] AUTHORIZATION REQUIRED FOR UPDATE\n";
        std::cout << "OTP: " << otp << "\n";

        std::string userInput;
        std::cout << "Enter OTP to confirm: ";
        std::cin >> userInput;

        if (userInput == otp) {
            std::string filePath = DOCS_DIR + "/" + filename;
            std::ofstream file(filePath, std::ios::app); 
            if (file.is_open()) {
                file << newData;
                file.close();
                initialize(); 
                Utils::printSuccess("Update successful and blockchain re-sealed.");
                AuditLogger::getInstance().log("USER", "Updated document: " + filename);
            }
        } else {
            Utils::printError("INVALID OTP. UPDATE ABORTED.");
        }
    }

    void authorizedDelete(const std::string& filename) {
        std::string otp = generateOTP();
        std::cout << "\n[AUTH] AUTHORIZATION REQUIRED FOR DELETION\n";
        std::cout << "OTP: " << otp << "\n";

        std::string userInput;
        std::cout << "Enter OTP to confirm: ";
        std::cin >> userInput;

        if (userInput == otp) {
            std::string filePath = DOCS_DIR + "/" + filename;
            std::string backupPath = BACKUP_DIR + "/" + filename;
            
            if (fs::exists(filePath)) {
                fs::remove(filePath);
                if (fs::exists(backupPath)) fs::remove(backupPath);
                initialize();
                Utils::printSuccess("Document removed and system re-sealed.");
                AuditLogger::getInstance().log("USER", "Deleted document: " + filename);
            } else {
                Utils::printError("File does not exist.");
            }
        } else {
            Utils::printError("INVALID OTP. DELETION ABORTED.");
        }
    }

    void authorizedOverwrite(const std::string& filename, const std::string& content) {
        std::string otp = generateOTP();
        std::cout << "\n[AUTH] AUTHORIZATION REQUIRED FOR OVERWRITE\n";
        std::cout << "OTP: " << otp << "\n";

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
                Utils::printSuccess("Document overwritten and sealed.");
                AuditLogger::getInstance().log("USER", "Overwrote document: " + filename);
            }
        } else {
            Utils::printError("INVALID OTP. OVERWRITE ABORTED.");
        }
    }

    void searchContent(const std::string& query) {
        Utils::printHeader("SEARCH RESULTS FOR: " + query);
        bool found = false;
        for (const auto& b : chain) {
            std::string filePath = DOCS_DIR + "/" + b.getFilename();
            std::ifstream file(filePath);
            std::string line;
            int lineNum = 1;
            while (std::getline(file, line)) {
                if (line.find(query) != std::string::npos) {
                    std::cout << "  [" << b.getFilename() << ":" << lineNum << "] " << line << "\n";
                    found = true;
                }
                lineNum++;
            }
        }
        if (!found) std::cout << "  No matches found.\n";
        Utils::printSeparator();
    }

    void restoreAll() {
        Utils::printHeader("RESTORING FROM BACKUP");
        for (const auto& block : chain) {
            std::string originalPath = DOCS_DIR + "/" + block.getFilename();
            std::string backupPath = BACKUP_DIR + "/" + block.getFilename();
            if (fs::exists(backupPath)) {
                fs::copy_file(backupPath, originalPath, fs::copy_options::overwrite_existing);
                std::cout << "  [RESTORED] " << block.getFilename() << "\n";
            }
        }
        Utils::printSuccess("System files restored to original state.");
        AuditLogger::getInstance().log("ADMIN", "System-wide restore performed");
    }

    void verify() {
        if (!loadLedger()) {
            Utils::printError("No ledger found. Initialize first.");
            return;
        }

        Utils::printLoadingBar("Verifying Blockchain Integrity", 800);
        Utils::printHeader("DEPARTMENT DOCUMENTS INTEGRITY REPORT");
        bool breach = false;
        std::string expectedPrev = "00000000";

        for (const auto& block : chain) {
            std::string filePath = DOCS_DIR + "/" + block.getFilename();
            
            if (block.getPrevBlockHash() != expectedPrev) {
                std::cout << "  [BROKEN CHAIN] " << block.getFilename() << "\n";
                breach = true;
            }
            expectedPrev = block.getOwnBlockHash();

            if (!Utils::fileExists(filePath)) {
                std::cout << "  [MISSING]  " << block.getFilename() << "\n";
                breach = true;
            } else {
                std::string currentHash = Hasher::calculateFileHash(filePath);
                if (currentHash != block.getFileHash()) {
                    std::cout << "  [TAMPERED] " << block.getFilename() << "\n";
                    breach = true;
                } else {
                    time_t t = block.getTimestamp();
                    std::string timeStr = std::ctime(&t);
                    if (!timeStr.empty() && timeStr.back() == '\n') timeStr.pop_back();
                    std::cout << "  [SECURE]   " << block.getFilename() << " (" << timeStr << ")\n";
                }
            }
        }

        if (breach) {
            std::cout << "\n  !!! BREACH DETECTED !!!\n";
            AuditLogger::getInstance().log("SYSTEM", "Integrity breach detected!");
        } else {
            Utils::printSuccess("All documents are secure and verified.");
            AuditLogger::getInstance().log("SYSTEM", "Integrity check passed");
        }
    }

    std::vector<std::string> getFileList() {
        std::vector<std::string> files;
        for (const auto& b : chain) files.push_back(b.getFilename());
        return files;
    }
};

#endif
