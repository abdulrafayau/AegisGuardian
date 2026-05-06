#ifndef HASHER_H
#define HASHER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

class Hasher {
public:
    static std::string calculateFileHash(const std::string& path) {
        std::ifstream file(path, std::ios::binary);
        if (!file) return "ERROR";

        unsigned long hash = 5381;
        char buffer[4096];

        while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
            std::streamsize bytes = file.gcount();
            for (int i = 0; i < bytes; i++) {
                hash = ((hash << 5) + hash) + static_cast<unsigned char>(buffer[i]);
                hash ^= (hash >> 3);
            }
        }

        std::stringstream ss;
        ss << std::hex << std::setw(8) << std::setfill('0') << hash;
        return ss.str();
    }

    static std::string calculateStringHash(const std::string& input) {
        unsigned long hash = 5381;
        for (char c : input) {
            hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
            hash ^= (hash >> 3);
        }
        std::stringstream ss;
        ss << std::hex << std::setw(8) << std::setfill('0') << hash;
        return ss.str();
    }
};

#endif
