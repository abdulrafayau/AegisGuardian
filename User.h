#ifndef USER_H
#define USER_H

#include <string>
#include "Hasher.h"

/**
 * CONCEPT: INHERITANCE & ABSTRACTION
 * This is the 'Base Class'. It's like a template for all users.
 * We make it 'Abstract' by adding a pure virtual function (= 0).
 */
class User {
protected:
    std::string username;
    std::string passwordHash; // We store the hash, NOT the actual password (Security!)
    std::string role;

public:
    User(std::string name, std::string pass, std::string r) 
        : username(name), role(r) {
        // We convert the password to a hash immediately for safety
        passwordHash = Hasher::calculateStringHash(pass);
    }

    // A virtual destructor is important for proper memory cleanup in inheritance
    virtual ~User() {}

    // Getters to access private/protected data
    std::string getUsername() const { return username; }
    std::string getRole() const { return role; }

    // Check if the entered password matches the stored hash
    bool authenticate(const std::string& pass) const {
        return Hasher::calculateStringHash(pass) == passwordHash;
    }

    /**
     * CONCEPT: POLYMORPHISM (Virtual Function)
     * Each derived class (Admin/Staff) will implement this differently.
     * The '= 0' makes this class 'Abstract' (you cannot create a generic User).
     */
    virtual bool canAccessAdminTools() const = 0; 
};

/**
 * Derived Class: Admin
 * Inherits everything from User.
 */
class Admin : public User {
public:
    Admin(std::string name, std::string pass) : User(name, pass, "ADMIN") {}
    
    // Admin CAN access tools, so we return true
    bool canAccessAdminTools() const override { return true; }
};

/**
 * Derived Class: Staff
 * Inherits everything from User.
 */
class Staff : public User {
public:
    Staff(std::string name, std::string pass) : User(name, pass, "STAFF") {}
    
    // Staff CANNOT access tools, so we return false
    bool canAccessAdminTools() const override { return false; }
};

#endif
