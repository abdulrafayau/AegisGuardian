#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#endif

namespace fs = std::filesystem;

class Utils {
public:
    enum Color {
        RESET = 0,
        RED = 31,
        GREEN = 32,
        YELLOW = 33,
        BLUE = 34,
        MAGENTA = 35,
        CYAN = 36,
        WHITE = 37,
        BOLD = 1
    };

    static void setupConsole() {
        #ifdef _WIN32
        SetConsoleOutputCP(65001);
        SetConsoleCP(65001);
        #endif
    }

    static void setColor(Color color) {
        #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        int winColor = 7; // Default White
        switch(color) {
            case RED: winColor = 12; break;
            case GREEN: winColor = 10; break;
            case YELLOW: winColor = 14; break;
            case BLUE: winColor = 9; break;
            case MAGENTA: winColor = 13; break;
            case CYAN: winColor = 11; break;
            case BOLD: winColor = 15; break;
            default: winColor = 7;
        }
        SetConsoleTextAttribute(hConsole, winColor);
        #else
        std::cout << "\033[" << (int)color << "m";
        #endif
    }

    static void reset() {
        #ifdef _WIN32
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        #else
        std::cout << "\033[0m";
        #endif
    }

    static void printBanner() {
        setColor(CYAN);
        std::cout << R"(
   ______ __  __  ___   ____   ____  ____  ___   __   __
  / ____// / / / /   | / __ \ / __ \/  _/ /   | /  | / /
 / / __ / / / / / /| |/ /_/ // / / // /  / /| |/   |/ / 
/ /_/ // /_/ / / ___ // _, _// /_/ // /  / ___ // /|  /  
\____/ \____/ /_/  |_/_/ |_|/_____//___//_/  |_/_/ |_/   
        )" << "\n";
        setColor(YELLOW);
        std::cout << "      --- ADVANCED DOCUMENT SECURITY SYSTEM ---\n";
        reset();
    }

    static void printLoadingBar(const std::string& task, int durationMs = 500) {
        std::cout << "  " << task << " [";
        for (int i = 0; i < 20; ++i) {
            std::cout << (char)219;
            #ifdef _WIN32
            Sleep(durationMs / 20);
            #else
            usleep((durationMs / 20) * 1000);
            #endif
        }
        std::cout << "] Done!\n";
    }

    static void clear() {
        #ifdef _WIN32
        system("cls");
        #else
        system("clear");
        #endif
    }

    static void printHeader(const std::string& title) {
        setColor(BLUE);
        std::cout << "\n\xC9" << std::setfill('\xCD') << std::setw(42) << "\xBB\n";
        std::cout << "\xBA   " << std::setfill(' ') << std::left << std::setw(39) << title << "\xBA\n";
        std::cout << "\xC8" << std::setfill('\xCD') << std::setw(42) << "\xBC\n";
        reset();
    }

    static void printSeparator() {
        setColor(WHITE);
        std::cout << std::setfill('\xC4') << std::setw(44) << "\n" << std::setfill(' ');
        reset();
    }

    static bool fileExists(const std::string& path) {
        return fs::exists(path) && fs::is_regular_file(path);
    }

    static void formatBytes(uintmax_t bytes) {
        if (bytes < 1024) std::cout << bytes << " B";
        else if (bytes < 1024 * 1024) std::cout << bytes / 1024 << " KB";
        else std::cout << bytes / (1024 * 1024) << " MB";
    }

    static void printSuccess(const std::string& msg) {
        setColor(GREEN);
        std::cout << "  [SUCCESS] " << msg << "\n";
        reset();
    }

    static void printError(const std::string& msg) {
        setColor(RED);
        std::cout << "  [ERROR] " << msg << "\n";
        reset();
    }

    static void printWarning(const std::string& msg) {
        setColor(YELLOW);
        std::cout << "  [WARNING] " << msg << "\n";
        reset();
    }
};

#endif
