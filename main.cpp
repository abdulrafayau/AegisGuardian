#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <ctime>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#endif

namespace fs = std::filesystem;
using namespace std;

const string RESET   = "\033[0m";
const string BOLD    = "\033[1m";
const string RED     = "\033[31m";
const string GREEN   = "\033[32m";
const string YELLOW  = "\033[33m";
const string MAGENTA = "\033[35m";
const string CYAN    = "\033[36m";
const string WHITE   = "\033[37m";
const string GRAY    = "\033[90m";

void clearScreen() {
#if defined(_WIN32)
    system("cls");
#else
    system("clear");
#endif
}

void sleepMs(int ms) {
    this_thread::sleep_for(chrono::milliseconds(ms));
}

void printBanner() {
    cout << CYAN << BOLD << R"(
    █████╗ ███████╗ ██████╗ ██╗███████╗
   ██╔══██╗██╔════╝██╔════╝ ██║██╔════╝
   ███████║█████╗  ██║  ███╗██║███████╗
   ██╔══██║██╔══╝  ██║   ██║██║╚════██║
   ██║  ██║███████╗╚██████╔╝██║███████║
   ╚═╝  ╚═╝╚══════╝ ╚═════╝ ╚═╝╚══════╝
    )" << RESET << "\n";
    cout << YELLOW << "      >>> ADVANCED SECURITY TERMINAL <<<" << RESET << "\n\n";
}

void checkExit(const string& input) {
    if (input == "e" || input == "E") {
        cout << RED << "\n[!] SYSTEM ABORT INITIATED. Goodbye.\n" << RESET;
        exit(0);
    }
}

string prompt(const string& text) {
    string input;
    cout << MAGENTA << "  [?] " << text << RESET;
    cin >> input;
    checkExit(input);
    return input;
}

void printStatus(const string& type, const string& msg) {
    if (type == "SUCCESS") cout << GREEN << "  [+] " << msg << RESET << "\n";
    else if (type == "ERROR") cout << RED << "  [!] " << msg << RESET << "\n";
    else if (type == "INFO") cout << CYAN << "  [i] " << msg << RESET << "\n";
    else if (type == "WARNING") cout << YELLOW << "  [*] " << msg << RESET << "\n";
    sleepMs(800);
}

class User {
public:
    string name, email, pass, role, luckyNumber;
    User(string n, string e, string p, string r, string ln) : name(n), email(e), pass(p), role(r), luckyNumber(ln) {}
    virtual ~User() {}
};

class Admin : public User {
public: Admin(string n, string e, string p, string ln) : User(n, e, p, "ADMIN", ln) {}
};

class Staff : public User {
public: Staff(string n, string e, string p, string ln) : User(n, e, p, "STAFF", ln) {}
};

class AegisSystem {
private:
    vector<User*> users;
    User* currentUser = nullptr;
    const string USER_FILE = "users.dat";
    const string LOG_FILE = "audit.log";

    void simulateBoot() {
        clearScreen();
        cout << GREEN << "Initializing Aegis Kernel...\n"; sleepMs(300);
        cout << "Loading Crypto Modules...\n"; sleepMs(300);
        cout << "Establishing Secure Connection...\n"; sleepMs(400);
        cout << "Boot Sequence Complete.\n" << RESET; sleepMs(200);
    }

    void log(string action) {
        ofstream f(LOG_FILE, ios::app);
        time_t now = time(0);
        char* dt = ctime(&now);
        string tStr(dt); if(!tStr.empty() && tStr.back() == '\n') tStr.pop_back();
        f << "[" << tStr << "] " << (currentUser ? currentUser->name : "SYSTEM") << " -> " << action << endl;
    }

    bool isValidPass(const string& p) { return p.length() >= 6 && p.length() <= 8; }

public:
    AegisSystem() {
        simulateBoot();
        loadUsers();
        if (!fs::exists("Vault")) fs::create_directory("Vault");
        if (!fs::exists(".backups")) fs::create_directory(".backups");
    }
    ~AegisSystem() { for (auto u : users) delete u; }

    void loadUsers() {
        if (!fs::exists(USER_FILE)) {
            users.push_back(new Admin("adminqar", "admin@aegis.com", "oop01", "1"));
            saveUsers();
            return;
        }
        ifstream f(USER_FILE);
        string n, e, p, r, ln;
        while (f >> n >> e >> p >> r >> ln) {
            if (r == "ADMIN") users.push_back(new Admin(n, e, p, ln));
            else users.push_back(new Staff(n, e, p, ln));
        }
    }

    void saveUsers() {
        ofstream f(USER_FILE);
        for (auto u : users) f << u->name << " " << u->email << " " << u->pass << " " << u->role << " " << u->luckyNumber << "\n";
    }

    bool login(string n, string p) {
        for (auto u : users) {
            if (u->name == n && u->pass == p) { currentUser = u; log("Logged in"); return true; }
        }
        return false;
    }

    void signup() {
        cout << "\n" << CYAN << "  --- NEW STAFF REGISTRATION ---" << RESET << "\n";
        string n = prompt("Choose Username: ");
        string e = prompt("Enter Email: ");
        string p = prompt("Choose Password (6-8 chars): ");
        string ln = prompt("Security Question - Lucky Number: ");

        for (auto u : users) {
            if (u->name == n) { printStatus("ERROR", "Username already exists."); return; }
        }
        if (!isValidPass(p)) { printStatus("ERROR", "Invalid password length."); return; }

        users.push_back(new Staff(n, e, p, ln));
        saveUsers();
        log("Registered new user: " + n);
        printStatus("SUCCESS", "Account registered successfully.");
    }

    void forgotPassword() {
        cout << "\n" << YELLOW << "  --- PASSWORD RECOVERY ---" << RESET << "\n";
        string n = prompt("Target Username: ");
        string ln = prompt("Security Question - Lucky Number: ");

        for (auto u : users) {
            if (u->name == n) {
                if (u->luckyNumber == ln) {
                    string newP = prompt("New Password (6-8 chars): ");
                    if (isValidPass(newP)) {
                        u->pass = newP; saveUsers();
                        log("Recovered password for: " + n);
                        printStatus("SUCCESS", "Password Reset Complete.");
                    } else printStatus("ERROR", "Invalid password length.");
                } else printStatus("ERROR", "Security Number Incorrect.");
                return;
            }
        }
        printStatus("ERROR", "User not found.");
    }

    void viewFiles() {
        cout << "\n" << CYAN << "  --- VAULT CONTENTS ---\n" << RESET;
        if (fs::is_empty("Vault")) { printStatus("INFO", "The vault is currently empty."); return; }
        for (const auto& entry : fs::directory_iterator("Vault")) {
            cout << GREEN << "  >> " << entry.path().filename().string() << " (" << fs::file_size(entry) << " bytes)\n" << RESET;
        }
    }

    void searchContent() {
        string keyword = prompt("Enter Search Keyword: ");
        cout << CYAN << "\n  --- SCAN RESULTS ---\n" << RESET;
        bool found = false;
        for (const auto& entry : fs::directory_iterator("Vault")) {
            ifstream f(entry.path()); string line; int num = 1;
            while (getline(f, line)) {
                if (line.find(keyword) != string::npos) {
                    cout << "  [" << entry.path().filename().string() << "] Line " << num << ": " << line << "\n";
                    found = true;
                }
                num++;
            }
        }
        if (!found) cout << "  Keyword not found.\n";
        log("Scanned for keyword: " + keyword);
    }

    void verifyIntegrity() {
        cout << "\n" << YELLOW << "  [SYSTEM] Running Deep Integrity Scan...\n" << RESET; sleepMs(800);
        if (fs::is_empty("Vault")) { printStatus("INFO", "No files to scan."); return; }
        for (const auto& entry : fs::directory_iterator("Vault")) {
            string name = entry.path().filename().string();
            string backup = ".backups/" + name;
            if (!fs::exists(backup)) cout << MAGENTA << "  [UNTRACKED] " << RESET << name << "\n";
            else if (fs::file_size(entry) != fs::file_size(backup)) cout << RED << "  [BREACHED]  " << RESET << name << "\n";
            else cout << GREEN << "  [SECURE]    " << RESET << name << "\n";
        }
        log("Ran integrity check");
    }

    void addOrUpdateFile(int mode) {
        string name = prompt("Filename: ");
        string path = "Vault/" + name;
        if (mode == 1 && fs::exists(path)) { printStatus("ERROR", "File already exists."); return; }
        if ((mode == 2 || mode == 3) && !fs::exists(path)) { printStatus("ERROR", "File not found."); return; }

        cout << MAGENTA << "  [?] Content (Type 'e' to cancel): " << RESET;
        string data; cin.ignore(); getline(cin, data); checkExit(data);

        ofstream f;
        if (mode == 2) f.open(path, ios::app);
        else f.open(path, ios::trunc);
        f << data << "\n"; f.close();

        fs::copy_file(path, ".backups/" + name, fs::copy_options::overwrite_existing);
        log((mode == 1 ? "Created " : mode == 2 ? "Appended " : "Overwrote ") + name);
        printStatus("SUCCESS", "Document Secured.");
    }

    void renameDoc() {
        string oldName = prompt("Current Filename: ");
        if (!fs::exists("Vault/" + oldName)) { printStatus("ERROR", "File not found."); return; }
        string newName = prompt("New Filename: ");
        fs::rename("Vault/" + oldName, "Vault/" + newName);
        if (fs::exists(".backups/" + oldName)) fs::rename(".backups/" + oldName, ".backups/" + newName);
        log("Renamed " + oldName + " to " + newName);
        printStatus("SUCCESS", "Document Renamed.");
    }

    void deleteDoc() {
        string name = prompt("Filename to delete: ");
        if (!fs::exists("Vault/" + name)) { printStatus("ERROR", "File not found."); return; }
        fs::remove("Vault/" + name);
        fs::remove(".backups/" + name);
        log("Deleted document: " + name);
        printStatus("SUCCESS", "Document Purged.");
    }

    void restoreSystem() {
        for (auto& e : fs::directory_iterator(".backups"))
            fs::copy_file(e.path(), "Vault/" + e.path().filename().string(), fs::copy_options::overwrite_existing);
        log("Restored system from backups");
        printStatus("SUCCESS", "System Restored to Safe State.");
    }

    void resealSystem() {
        string confirm = prompt("WARNING: Clears ALL data. Type 'CONFIRM': ");
        if (confirm == "CONFIRM") {
            fs::remove_all("Vault"); fs::create_directory("Vault");
            fs::remove_all(".backups"); fs::create_directory(".backups");
            fs::remove(LOG_FILE);
            log("SYSTEM RE-SEALED");
            printStatus("SUCCESS", "System Re-initialized.");
        } else printStatus("INFO", "Aborted.");
    }

    void viewLogs() {
        if (!fs::exists(LOG_FILE)) { printStatus("INFO", "No logs found."); return; }
        cout << GRAY << "\n  --- AUDIT LOGS ---\n";
        ifstream f(LOG_FILE); string line;
        while (getline(f, line)) cout << "  " << line << "\n";
        cout << RESET;
    }

    void securityInfo() {
        cout << CYAN << "\n  --- ACTIVE SESSION INFO ---\n" << RESET;
        cout << "  User: " << currentUser->name << "\n  Role: " << currentUser->role << "\n  Email: " << currentUser->email << "\n";
        int files = 0; for (const auto& entry : fs::directory_iterator("Vault")) files++;
        cout << "  Secured Documents: " << files << "\n";
    }

    User* getUser() { return currentUser; }
    void logout() { if(currentUser) log("Logged out"); currentUser = nullptr; printStatus("INFO", "Logged out."); }
};

void printDashboard(User* u) {
    clearScreen();
    printBanner();
    cout << "  " << CYAN << "[ACTIVE SESSION] " << u->name << " (" << u->role << ")" << RESET << "\n\n";
    cout << "  " << MAGENTA << "--- DOCUMENT MANAGEMENT SYSTEM ---" << RESET << "\n";
    cout << "  1. Explore Vault & View\n  2. Search Content (Deep Scan)\n  3. Verify System Integrity\n\n";
    cout << "  " << CYAN << "--- MODIFICATION TOOLS ---" << RESET << "\n";
    cout << "  4. New Document (Create/Paste)\n  5. Append Data (Update)\n  6. Replace Data (Overwrite)\n  7. Rename Document\n\n";

    if (u->role == "ADMIN") {
        cout << "  " << RED << "--- ADMINISTRATIVE TOOLS ---" << RESET << "\n";
        cout << "  8. Delete Document\n  9. Self-Healing (Restore)\n  10. Re-Seal System (Re-initialize)\n  11. View Audit Logs\n\n";
    }

    cout << "  " << GRAY << "--- SESSION ---" << RESET << "\n";
    cout << "  12. Security Info\n  13. Logout\n  14. Exit\n\n";
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    AegisSystem sys;
    string choice;

    while (true) {
        if (!sys.getUser()) {
            clearScreen();
            printBanner();
            cout << WHITE << "  +------------------------------------+\n";
            cout << "  | 1. System Login                    |\n";
            cout << "  | 2. Register New Staff              |\n";
            cout << "  | 3. Forgot Password                 |\n";
            cout << "  | E. Exit System                     |\n";
            cout << "  +------------------------------------+" << RESET << "\n\n";

            choice = prompt("Select Protocol: ");
            if (choice == "1") {
                string n = prompt("Username: "); string p = prompt("Password: ");
                if (!sys.login(n, p)) printStatus("ERROR", "Authentication Failed.");
            }
            else if (choice == "2") sys.signup();
            else if (choice == "3") sys.forgotPassword();
        } else {
            printDashboard(sys.getUser());
            choice = prompt("Select Directive: ");
            bool isAdmin = (sys.getUser()->role == "ADMIN");

            cout << "\n";
            if (choice == "1") sys.viewFiles();
            else if (choice == "2") sys.searchContent();
            else if (choice == "3") sys.verifyIntegrity();
            else if (choice == "4") sys.addOrUpdateFile(1);
            else if (choice == "5") sys.addOrUpdateFile(2);
            else if (choice == "6") sys.addOrUpdateFile(3);
            else if (choice == "7") sys.renameDoc();
            else if (choice == "8" && isAdmin) sys.deleteDoc();
            else if (choice == "9" && isAdmin) sys.restoreSystem();
            else if (choice == "10" && isAdmin) sys.resealSystem();
            else if (choice == "11" && isAdmin) sys.viewLogs();
            else if (choice == "12") sys.securityInfo();
            else if (choice == "13") sys.logout();
            else if (choice == "14") exit(0);
            else printStatus("ERROR", "Invalid Directive.");

            if (sys.getUser()) { cout << "\n"; prompt("Press 'c' to continue: "); }
        }
    }
    return 0;
}