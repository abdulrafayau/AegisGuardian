#ifndef AUDITLOGGER_H
#define AUDITLOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include "Utils.h"

class AuditLogger {
private:
    const std::string LOG_FILE = "audit.log";
    
    AuditLogger() {} // Private constructor (Singleton)

public:
    static AuditLogger& getInstance() {
        static AuditLogger instance;
        return instance;
    }

    void log(const std::string& user, const std::string& action) {
        std::ofstream file(LOG_FILE, std::ios::app);
        if (file.is_open()) {
            time_t now = time(0);
            char* dt = ctime(&now);
            std::string timeStr(dt);
            if (!timeStr.empty() && timeStr.back() == '\n') timeStr.pop_back();

            file << "[" << timeStr << "] USER: " << user << " | ACTION: " << action << "\n";
            file.close();
        }
    }

    void viewLogs() {
        std::ifstream file(LOG_FILE);
        if (file.is_open()) {
            Utils::printHeader("SYSTEM AUDIT LOGS");
            std::string line;
            while (std::getline(file, line)) {
                std::cout << "  " << line << "\n";
            }
            Utils::printSeparator();
        } else {
            Utils::printError("No audit logs found.");
        }
    }

    // Delete copy constructor and assignment operator
    AuditLogger(const AuditLogger&) = delete;
    void operator=(const AuditLogger&) = delete;
};

#endif
