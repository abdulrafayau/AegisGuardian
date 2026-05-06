#ifndef USER_H
#define USER_H

#include <string>
#include "Hasher.h"

class User {
protected:
    std::string username;
    std::string passwordHash; 
    std::string role;

public:
    User(std::string name, std::string pass, std::string r) 
        : username(name), role(r) {
        passwordHash = Hasher::calculateStringHash(pass);
    }

    // Constructor for loading from file (pre-hashed)
    User(std::string name, std::string passHash, std::string r, bool isAlreadyHashed)
        : username(name), passwordHash(passHash), role(r) {}

    virtual ~User() {}

    std::string getUsername() const { return username; }
    std::string getRole() const { return role; }
    std::string getPasswordHash() const { return passwordHash; }

    bool authenticate(const std::string& pass) const {
        return Hasher::calculateStringHash(pass) == passwordHash;
    }

    virtual bool canAccessAdminTools() const = 0; 
};

class Admin : public User {
public:
    Admin(std::string name, std::string pass) : User(name, pass, "ADMIN") {}
    Admin(std::string name, std::string passHash, bool isAlreadyHashed) 
        : User(name, passHash, "ADMIN", true) {}
    
    bool canAccessAdminTools() const override { return true; }
};

class Staff : public User {
public:
    Staff(std::string name, std::string pass) : User(name, pass, "STAFF") {}
    Staff(std::string name, std::string passHash, bool isAlreadyHashed) 
        : User(name, passHash, "STAFF", true) {}
    
    bool canAccessAdminTools() const override { return false; }
};

#endif
