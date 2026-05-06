#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <vector>
#include <fstream>
#include <sstream>
#include "User.h"
#include "Utils.h"
#include "AuditLogger.h"

class UserManager {
private:
    std::vector<User*> users;
    User* currentUser;
    const std::string USER_FILE = "users.guardian";

    void saveUsers() {
        std::ofstream file(USER_FILE);
        for (auto u : users) {
            file << u->getUsername() << "|" << u->getPasswordHash() << "|" << u->getRole() << "\n";
        }
    }

    void loadUsers() {
        if (!Utils::fileExists(USER_FILE)) {
            // Pre-populate with requested users
            users.push_back(new Admin("admin", "admin123"));
            users.push_back(new Admin("adminrafay", "rafay123"));
            users.push_back(new Staff("staff", "staff123"));
            saveUsers();
            AuditLogger::getInstance().log("SYSTEM", "Initialized default users");
            return;
        }

        std::ifstream file(USER_FILE);
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string name, hash, role;
            std::getline(ss, name, '|');
            std::getline(ss, hash, '|');
            std::getline(ss, role, '|');

            if (name.empty()) continue;

            if (role == "ADMIN") users.push_back(new Admin(name, hash, true));
            else users.push_back(new Staff(name, hash, true));
        }
    }

public:
    UserManager() : currentUser(nullptr) {
        loadUsers();
    }

    ~UserManager() {
        for (auto u : users) delete u;
    }

    User* getCurrentUser() { return currentUser; }
    void logout() { 
        if (currentUser) AuditLogger::getInstance().log(currentUser->getUsername(), "Logged out");
        currentUser = nullptr; 
    }

    bool login(const std::string& name, const std::string& pass) {
        for (auto u : users) {
            if (u->getUsername() == name && u->authenticate(pass)) {
                currentUser = u;
                AuditLogger::getInstance().log(name, "Logged in successfully");
                return true;
            }
        }
        AuditLogger::getInstance().log(name, "Failed login attempt");
        return false;
    }

    bool registerUser(const std::string& name, const std::string& pass, const std::string& role) {
        for (auto u : users) {
            if (u->getUsername() == name) return false; // Duplicate
        }

        if (role == "ADMIN") users.push_back(new Admin(name, pass));
        else users.push_back(new Staff(name, pass));

        saveUsers();
        AuditLogger::getInstance().log(name, "Account registered as " + role);
        return true;
    }
};

#endif
