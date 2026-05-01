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
        // Set console output codepage to UTF-8 to prevent "Chinese" garbled text
        SetConsoleOutputCP(65001);
        SetConsoleCP(65001);
        #endif
    }

    static void setColor(Color color) {
        #ifdef _WIN32
        // Basic Windows support if ANSI is not enabled
        #endif
        std::cout << "\033[" << color << "m";
    }

    static void reset() {
        std::cout << "\033[0m";
    }

    static void clear() {
        #ifdef _WIN32
        system("cls");
        #else
        system("clear");
        #endif
    }

    static void printHeader(const std::string& title) {
        setColor(CYAN);
        std::cout << "\n========================================\n";
        std::cout << "   " << title << "\n";
        std::cout << "========================================\n";
        reset();
    }

    static void printSeparator() {
        setColor(CYAN);
        std::cout << "----------------------------------------\n";
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
        std::cout << "[SUCCESS] " << msg << "\n";
        reset();
    }

    static void printError(const std::string& msg) {
        setColor(RED);
        std::cout << "[ERROR] " << msg << "\n";
        reset();
    }

    static void printWarning(const std::string& msg) {
        setColor(YELLOW);
        std::cout << "[WARNING] " << msg << "\n";
        reset();
    }
};

#endif
