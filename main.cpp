#include "Aegis.h"
#include "UserManager.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

UserManager userMgr;

void showBanner() {
    Utils::printBanner();
    if (userMgr.getCurrentUser()) {
        Utils::setColor(Utils::CYAN);
        cout << "  [ACTIVE SESSION] ";
        Utils::setColor(Utils::BOLD);
        cout << userMgr.getCurrentUser()->getUsername();
        Utils::setColor(Utils::CYAN);
        cout << " (" << userMgr.getCurrentUser()->getRole() << ")\n";
        Utils::reset();
    }
}

bool handleLogin() {
    Utils::clear();
    showBanner();
    string name, pass;
    cout << "\n  --- SECURE SYSTEM LOGIN ---\n";
    cout << "  " << left << setw(10) << "Username:" << " "; cin >> name;
    cout << "  " << left << setw(10) << "Password:" << " "; cin >> pass;

    if (userMgr.login(name, pass)) {
        Utils::printSuccess("Authentication Successful. Terminal Access Granted.");
        system("pause");
        return true;
    }
    Utils::printError("Invalid Credentials. Access Denied.");
    system("pause");
    return false;
}

void handleSignup() {
    Utils::clear();
    showBanner();
    string name, pass, role;
    int roleChoice;
    cout << "\n  --- IDENTITY REGISTRATION ---\n";
    cout << "  " << left << setw(10) << "Username:" << " "; cin >> name;
    cout << "  " << left << setw(10) << "Password:" << " "; cin >> pass;
    
    Utils::setColor(Utils::YELLOW);
    cout << "  Select Designation:\n";
    cout << "  1. [ADMIN] Full System Control\n";
    cout << "  2. [STAFF] Document Operations Only\n";
    cout << "  Choice: "; 
    Utils::reset();
    cin >> roleChoice;
    
    role = (roleChoice == 1) ? "ADMIN" : "STAFF";

    if (role == "ADMIN") {
        string secret;
        cout << "  Enter System Secret Key: ";
        cin >> secret;
        if (secret != "guardian2026") {
            Utils::printError("Authorization Failed. Admin registration aborted.");
            system("pause");
            return;
        }
    }

    if (userMgr.registerUser(name, pass, role)) {
        Utils::printSuccess("Account Registered Successfully!");
    } else {
        Utils::printError("Identity Conflict: Username already taken.");
    }
    system("pause");
}

int selectFile(Aegis& aegis) {
    auto files = aegis.getFileList();
    if (files.empty()) {
        Utils::printWarning("Vault is empty. No documents found.");
        return -1;
    }
    Utils::setColor(Utils::BLUE);
    cout << "  --- SECURE DOCUMENT VAULT ---\n";
    Utils::reset();
    for(size_t i = 0; i < files.size(); ++i) {
        cout << "  [" << right << setw(2) << i + 1 << "] " << left << files[i] << "\n";
    }
    int fIndex; 
    cout << "\n  Select index (0 to cancel): "; 
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
    Aegis aegis;
    int choice;

    while (true) {
        if (!userMgr.getCurrentUser()) {
            Utils::clear();
            showBanner();
            Utils::setColor(Utils::BOLD);
            cout << "\n  1. Login to Vault\n  2. Register Identity\n  3. Terminate Session\n\n";
            Utils::reset();
            cout << "  Select Choice: ";
            if (!(cin >> choice)) {
                cin.clear(); cin.ignore(1000, '\n'); continue;
            }
            if (choice == 1) handleLogin();
            else if (choice == 2) handleSignup();
            else if (choice == 3) return 0;
            continue;
        }

        aegis.loadLedger();
        User* currentUser = userMgr.getCurrentUser();

        Utils::clear();
        showBanner();
        
        Utils::setColor(Utils::MAGENTA);
        cout << "\n  --- DOCUMENT MANAGEMENT SYSTEM ---\n";
        Utils::reset();
        cout << "  1.  Explore Vault & View\n";
        cout << "  2.  Search Content (Deep Scan)\n";
        cout << "  3.  Verify System Integrity\n";
        
        Utils::setColor(Utils::CYAN);
        cout << "\n  --- MODIFICATION TOOLS ---\n";
        Utils::reset();
        cout << "  4.  New Document (Create/Paste)\n";
        cout << "  5.  Append Data (Update)\n";
        cout << "  6.  Replace Data (Overwrite)\n";
        cout << "  7.  Rename Document\n";
        
        if (currentUser->canAccessAdminTools()) {
            Utils::setColor(Utils::RED);
            cout << "\n  --- ADMINISTRATIVE TOOLS ---\n";
            Utils::reset();
            cout << "  8.  Delete Document\n";
            cout << "  9.  Self-Healing (Restore)\n";
            cout << "  10. Re-Seal System (Re-initialize)\n";
            cout << "  11. View Audit Logs\n";
        }

        Utils::setColor(Utils::WHITE);
        cout << "\n  --- SESSION ---\n";
        Utils::reset();
        cout << "  12. Security Info\n";
        cout << "  13. Logout\n";
        cout << "  14. Exit\n";
        cout << "\n  Command >> ";
        
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(1000, '\n'); continue;
        }

        switch (choice) {
            case 1: {
                int idx = selectFile(aegis);
                if (idx != -1) aegis.viewFile(aegis.getFileList()[idx]);
                break;
            }
            case 2: {
                string query;
                cout << "  Enter Scan Query: "; cin.ignore(); getline(cin, query);
                aegis.searchContent(query);
                break;
            }
            case 3: aegis.verify(); break;
            case 4: {
                string filename; 
                cout << "  Target Filename: "; cin >> filename;
                cout << "  Enter Content (Ctrl+Z -> Enter to Finish):\n";
                aegis.addNewFile(filename, getMultilineInput());
                break;
            }
            case 5: {
                int idx = selectFile(aegis);
                if (idx != -1) {
                    cout << "  Enter Data to Append (Ctrl+Z -> Enter to Finish):\n";
                    aegis.authorizedUpdate(aegis.getFileList()[idx], getMultilineInput());
                }
                break;
            }
            case 6: {
                int idx = selectFile(aegis);
                if (idx != -1) {
                    cout << "  Enter New Content (Ctrl+Z -> Enter to Finish):\n";
                    aegis.authorizedOverwrite(aegis.getFileList()[idx], getMultilineInput());
                }
                break;
            }
            case 7: {
                int idx = selectFile(aegis);
                if (idx != -1) {
                    string newName;
                    cout << "  New Filename: "; cin >> newName;
                    aegis.authorizedRename(aegis.getFileList()[idx], newName);
                }
                break;
            }
            case 8:
                if (currentUser->canAccessAdminTools()) {
                    int idx = selectFile(aegis);
                    if (idx != -1) aegis.authorizedDelete(aegis.getFileList()[idx]);
                } else Utils::printError("Restricted Operation: Admins only.");
                break;
            case 9:
                if (currentUser->canAccessAdminTools()) aegis.restoreAll();
                else Utils::printError("Restricted Operation: Admins only.");
                break;
            case 10:
                if (currentUser->canAccessAdminTools()) aegis.initialize();
                else Utils::printError("Restricted Operation: Admins only.");
                break;
            case 11:
                if (currentUser->canAccessAdminTools()) AuditLogger::getInstance().viewLogs();
                else Utils::printError("Restricted Operation: Admins only.");
                break;
            case 12:
                Utils::printHeader("SECURITY CLEARANCE");
                cout << "  Active Role: " << currentUser->getRole() << "\n";
                cout << "  Permissions: " << (currentUser->canAccessAdminTools() ? "LEVEL 4 (SUPERUSER)" : "LEVEL 2 (STAFF)") << "\n";
                break;
            case 13: userMgr.logout(); break;
            case 14: return 0;
            default: cout << "  Invalid Command.\n";
        }
        system("pause");
    }

    return 0;
}
