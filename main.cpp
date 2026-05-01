#include "Guardian.h"
#include "User.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

User* currentUser = nullptr;
vector<User*> users;

void setupUsers() {
    users.push_back(new Admin("admin", "admin123"));
    users.push_back(new Staff("staff", "staff123"));
}

void showBanner() {
    Utils::setColor(Utils::CYAN);
    cout << "  " << setfill('-') << setw(48) << "-" << "\n";
    cout << "     " << left << setw(43) << setfill(' ') << "DEPARTMENT FILE GUARDIAN & DEFENDER (v5.0)" << "\n";
    cout << "     " << left << setw(43) << "Role-Based Access Control (RBAC) Active" << "\n";
    cout << "  " << setfill('-') << setw(48) << "-" << setfill(' ') << "\n";
    if (currentUser) {
        Utils::setColor(Utils::YELLOW);
        cout << "  Logged in as: " << left << setw(15) << currentUser->getUsername() 
             << " [" << currentUser->getRole() << "]\n";
    }
    Utils::reset();
}

bool login() {
    Utils::clear();
    showBanner();
    string name, pass;
    cout << "\n  --- SYSTEM LOGIN ---\n";
    cout << "  " << left << setw(10) << "Username:" << " "; cin >> name;
    cout << "  " << left << setw(10) << "Password:" << " "; cin >> pass;

    for (auto u : users) {
        if (u->getUsername() == name && u->authenticate(pass)) {
            currentUser = u;
            return true;
        }
    }
    Utils::printError("Invalid username or password!");
    system("pause");
    return false;
}

int selectFile(Guardian& guardian) {
    auto files = guardian.getFileList();
    if (files.empty()) {
        Utils::printWarning("No documents found.");
        return -1;
    }
    for(size_t i = 0; i < files.size(); ++i) {
        cout << "  " << right << setw(2) << i + 1 << ". " << left << files[i] << "\n";
    }
    int fIndex; 
    cout << "  Select file: "; 
    cin >> fIndex;
    if (fIndex > 0 && fIndex <= (int)files.size()) return fIndex - 1;
    return -1;
}

string getMultilineInput() {
    cin.ignore(); 
    string data = "", line;
    while(getline(cin, line)) data += line + "\n";
    cin.clear();
    return data;
}

int main() {
    Utils::setupConsole();
    setupUsers();
    Guardian guardian;
    int choice;

    while (!login());

    guardian.loadLedger();

    while (true) {
        Utils::clear();
        showBanner();
        
        cout << "  " << right << setw(2) << "1." << " " << left << "Verify Documents (Check for Breach)\n";
        cout << "  " << right << setw(2) << "2." << " " << left << "View Document (Secure Viewer)\n";
        cout << "  " << right << setw(2) << "3." << " " << left << "Authorized Data Entry (Bulk Paste + OTP)\n";
        cout << "  " << right << setw(2) << "4." << " " << left << "Authorized New Document (Bulk Create + OTP)\n";
        
        if (currentUser->canAccessAdminTools()) {
            Utils::setColor(Utils::MAGENTA);
            cout << "  " << right << setw(2) << "5." << " " << left << "[ADMIN] Initialize / Re-Seal System\n";
            cout << "  " << right << setw(2) << "6." << " " << left << "[ADMIN] Self-Healing (Restore Backups)\n";
            Utils::reset();
        }

        cout << "  " << right << setw(2) << "7." << " " << left << "Help / Info\n";
        cout << "  " << right << setw(2) << "8." << " " << left << "Logout / Switch User\n";
        cout << "  " << right << setw(2) << "9." << " " << left << "Exit\n";
        cout << "\n  Select Option: ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (choice) {
            case 1: 
                guardian.verify(); 
                break;
            case 2: {
                int idx = selectFile(guardian);
                if (idx != -1) guardian.viewFile(guardian.getFileList()[idx]);
                break;
            }
            case 3: {
                int idx = selectFile(guardian);
                if (idx != -1) {
                    cout << "  Enter text to add (press Ctrl+Z then Enter to finish):\n";
                    guardian.authorizedUpdate(guardian.getFileList()[idx], getMultilineInput());
                }
                break;
            }
            case 4: {
                string filename; 
                cout << "  Enter name: "; 
                cin >> filename;
                cout << "  Enter content (press Ctrl+Z then Enter to finish):\n";
                guardian.addNewFile(filename, getMultilineInput());
                break;
            }
            case 5:
                if (currentUser->canAccessAdminTools()) guardian.initialize();
                else Utils::printError("ACCESS DENIED: Admins only.");
                break;
            case 6:
                if (currentUser->canAccessAdminTools()) guardian.restoreAll();
                else Utils::printError("ACCESS DENIED: Admins only.");
                break;
            case 7:
                Utils::printHeader("SECURITY INFO");
                cout << "  " << left << setw(10) << "Role:" << currentUser->getRole() << "\n";
                cout << "  " << left << setw(10) << "Access:" << (currentUser->canAccessAdminTools() ? "Full Control" : "Limited") << "\n";
                break;
            case 8:
                currentUser = nullptr;
                while (!login());
                break;
            case 9:
                for (auto u : users) delete u;
                return 0;
            default: 
                cout << "  Invalid choice.\n";
        }
        system("pause");
    }

    return 0;
}
