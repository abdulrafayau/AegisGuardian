# 🛡️ Aegis Advanced Version Control & Security (v6.0)

A professional-grade, creative, and blockchain-based Document Management System (DMS) developed in C++ using advanced Object-Oriented Programming (OOP) principles.

## 🚀 Overview
**Aegis** is a high-security vault for sensitive documents. It combines traditional document management (CRUD) with a "Chain of Trust" architecture. Every file modification is sealed into a blockchain ledger, ensuring absolute data integrity. If a file is tampered with, the system detects it instantly and offers self-healing capabilities.

### Key Features
- **Full CRUD Management**: Create, View, Update, Overwrite, Rename, and Delete documents in the `Docs` directory.
*   **Blockchain Data Integrity**: Uses a linked-hash structure where every modification re-seals the entire chain.
*   **Persistent User Management**: Securely stores users and their roles with hashed password protection.
- **Role-Based Access Control (RBAC)**: Distinct permissions for `Level 4 (Admin)` and `Level 2 (Staff)` users.
- **Creative Terminal UI**: ANSI-colored interface, ASCII banners, and professional loading animations.
*   **Secure Audit Logging**: Every system action is recorded in a protected `audit.log` for forensic tracking.
- **OTP Authorization**: Destructive actions require a One-Time Password to prevent accidental changes.

---

## 📊 Class Architecture (UML)

# 🛡️ Aegis Guardian

**Aegis Guardian** is an advanced, terminal-based Document Security and Management Operating System built entirely in C++. Designed as a secure vault simulation, it features Role-Based Access Control (RBAC), self-healing file integrity algorithms, and a sleek ANSI-colored Command Line Interface.

---

## 🏛️ System Architecture (UML)

The system relies on a modular, object-oriented architecture. Below is the UML Class Diagram illustrating the structure and relationships of the core entities.

```mermaid
classDiagram
    class User {
        +string name
        +string email
        +string pass
        +string role
        +string luckyNumber
        +User(n, e, p, r, ln)
        +virtual ~User()
    }

    class Admin {
        +Admin(n, e, p, ln)
    }

    class Staff {
        +Staff(n, e, p, ln)
    }

    class AegisSystem {
        -vector~User*~ users
        -User* currentUser
        -const string USER_FILE
        -const string LOG_FILE
        -simulateBoot()
        -log(string action)
        -isValidPass(string p)
        +AegisSystem()
        +~AegisSystem()
        +loadUsers()
        +saveUsers()
        +login(string n, string p)
        +signup()
        +forgotPassword()
        +viewFiles()
        +searchContent()
        +verifyIntegrity()
        +addOrUpdateFile(int mode)
        +renameDoc()
        +deleteDoc()
        +restoreSystem()
        +resealSystem()
        +viewLogs()
        +securityInfo()
        +getUser() User*
        +logout()
    }

    User <|-- Admin : Inherits
    User <|-- Staff : Inherits
    AegisSystem "1" *-- "many" User : Aggregates / Manages

## 🛠️ OOP Concepts Applied

| Concept | Implementation in this Project |
|---------|--------------------------------|
| **Abstraction** | `User` class is abstract; `AuditLogger` uses a private constructor (Singleton). |
| **Inheritance** | `Admin` and `Staff` extend `User` to define role-specific permissions. |
| **Polymorphism** | Dynamic menu rendering based on `User*` pointers and virtual methods. |
| **Encapsulation** | Strict data hiding in `FileBlock`, `UserManager`, and `AuditLogger`. |
| **Singleton** | The `AuditLogger` ensures a single point of truth for system activity. |

---

## 🛠️ Compilation & Execution

1. **Prerequisites**: G++ compiler with C++17 support.
2. **Compile**: 
   ```bash
   g++ main.cpp -o Aegis.exe -std=c++17
   ```
3. **Run**:
   ```bash
   .\Aegis.exe
---

### Default Credentials:
- **Admin**: `admin` / `admin123`
- **Admin 2**: `adminrafay` / `rafay123`
- **Staff**: `staff` / `staff123`
- **Admin Secret Key**: `guardian2026` (For registration)

### Group Members :
- Abdul Rafay 
- Hadia Aziz
- Huda Tariq
- Faizan Iqbal

  Here is the fully upgraded `README.md` file. I have added a beautifully structured **Mermaid.js UML Diagram** (which GitHub natively supports and renders as a visual graphic) and a dedicated section explaining exactly how the four pillars of Object-Oriented Programming were implemented in the code.

Copy this text and replace the contents of your `README.md` file on GitHub:

```markdown
# 🛡️ Aegis Guardian

**Aegis Guardian** is an advanced, terminal-based Document Security and Management Operating System built entirely in C++. Designed as a secure vault simulation, it features Role-Based Access Control (RBAC), self-healing file integrity algorithms, and a sleek ANSI-colored Command Line Interface.

---

## 🏛️ System Architecture (UML)

The system relies on a modular, object-oriented architecture. Below is the UML Class Diagram illustrating the structure and relationships of the core entities.

```mermaid
classDiagram
    class User {
        +string name
        +string email
        +string pass
        +string role
        +string luckyNumber
        +User(n, e, p, r, ln)
        +virtual ~User()
    }

    class Admin {
        +Admin(n, e, p, ln)
    }

    class Staff {
        +Staff(n, e, p, ln)
    }

    class AegisSystem {
        -vector~User*~ users
        -User* currentUser
        -const string USER_FILE
        -const string LOG_FILE
        -simulateBoot()
        -log(string action)
        -isValidPass(string p)
        +AegisSystem()
        +~AegisSystem()
        +loadUsers()
        +saveUsers()
        +login(string n, string p)
        +signup()
        +forgotPassword()
        +viewFiles()
        +searchContent()
        +verifyIntegrity()
        +addOrUpdateFile(int mode)
        +renameDoc()
        +deleteDoc()
        +restoreSystem()
        +resealSystem()
        +viewLogs()
        +securityInfo()
        +getUser() User*
        +logout()
    }

    User <|-- Admin : Inherits
    User <|-- Staff : Inherits
    AegisSystem "1" *-- "many" User : Aggregates / Manages
```

This project heavily implements the four core pillars of Object-Oriented Programming to ensure a secure, scalable, and memory-efficient codebase:

### 1. Encapsulation (Data Hiding)
Sensitive system data and helper functions are hidden from the outside world using the `private` access modifier inside the `AegisSystem` class.
* **Example:** Variables like `vector<User*> users` (the database of accounts) and `LOG_FILE` are private. The main program cannot alter them directly; it must use public interfaces like `signup()` or `login()` to interact with the data safely.

### 2. Inheritance
To promote code reusability, the system uses a hierarchical class structure for account types.
* **Example:** The `Admin` and `Staff` classes inherit from the base `User` class. Instead of rewriting properties like `name`, `email`, and `password` for every type of account, the derived classes simply inherit them and automatically pass their specific access level (`"ADMIN"` or `"STAFF"`) to the base constructor.

### 3. Polymorphism
The system handles different types of objects dynamically at runtime, specifically focusing on safe memory deallocation.
* **Example:** The base `User` class features a **Virtual Destructor** (`virtual ~User() {}`). When the `AegisSystem` shuts down and loops through its `vector<User*>` to delete objects, the virtual destructor ensures that C++ figures out at runtime whether it is destroying an `Admin` or a `Staff` member, preventing memory leaks.

### 4. Abstraction
Complex, low-level background operations are entirely hidden from the user interface.
* **Example:** When an admin chooses to restore the system, they simply call the public `sys.restoreSystem()` method. All the complex `<filesystem>` iterations, directory pathing, and byte-overwriting options are abstracted away behind that single, clean function call.

---

## ✨ Core Features

* **Role-Based Access Control (RBAC):** Strict separation of privileges. `STAFF` members can explore and read documents, while `ADMIN` accounts possess the authority to create, modify, delete, and restore system states.
* **Self-Healing Vault Integrity:** Every document created in the Vault is secretly mirrored. The Deep Scan Integrity feature checks byte-sizes to detect tampering, unauthorized modifications, or breaches.
* **Vim-Style Multi-Line Editor:** Create and append documents natively in the terminal with multi-line support, saving safely via `:wq` commands.
* **Disaster Recovery (Restore & Re-seal):** Admins can execute system-wide rollbacks to restore breached files from hidden backups, or "Re-seal" the entire OS to wipe all data securely.
* **Cryptographic-Style Authentication:** Secure login system with an integrated signup protocol. Includes a "Forgot Password" override utilizing a secure Lucky Number prompt.
* **Persistent Audit Logging:** Every system interaction—from logins to file deletions—is permanently logged in `audit.log` with an exact timestamp for administrative review.
* **Emergency Exit Protocol:** A globally active listener that instantly aborts the system safely if compromised.

---

## 🛠️ Technical Stack
* **Language:** C++ (Requires C++17 or higher)
* **File Operations:** `<filesystem>`, `<fstream>`
* **UI/UX:** Native ANSI escape sequences for terminal colorization and formatting.
* **Encoding:** Enforced UTF-8 output (`windows.h`) to ensure cross-platform ASCII art rendering.

---

## 🚀 Installation & Compilation

Ensure you have a C++ compiler (like GCC/MinGW) installed that supports at least C++17.

**1. Clone the repository:**
```bash
git clone [https://github.com/abdulrafayau/AegisGuardian.git](https://github.com/abdulrafayau/AegisGuardian.git)
cd AegisGuardian
```

**2. Compile the source code:**
```bash
g++ main.cpp -o aegis -std=c++17
```

**3. Run the OS:**
```bash
./aegis
```

---

## 🔑 Default Credentials
On the very first launch, Aegis automatically generates a secure master administrative account:
* **Username:** `admin`
* **Password:** `oop01`
* **Lucky Number:** `1`

*(Note: Custom passwords created via the Signup protocol must be strictly between 6 and 8 characters).*
```

