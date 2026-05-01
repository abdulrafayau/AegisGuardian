#ifndef HASHER_H
#define HASHER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

/**
 * CONCEPT: ENCAPSULATION & UTILITY CLASS
 * This class handles all the "Cyber" math. It's a utility class
 * because all its methods are 'static' (you don't need to create a Hasher object).
 */
class Hasher {
public:
    /**
     * This function reads a file byte-by-byte and turns it into a unique 8-character code (Hash).
     * If even one letter in the file changes, this code will change completely!
     */
    static std::string calculateFileHash(const std::string& path) {
        // Open file in binary mode to read raw data accurately
        std::ifstream file(path, std::ios::binary);
        if (!file) return "ERROR";

        unsigned long hash = 5381; // Starting seed for the DJB2 algorithm
        char buffer[4096];

        // Read the file in chunks (blocks) for efficiency
        while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
            std::streamsize bytes = file.gcount();
            for (int i = 0; i < bytes; i++) {
                // MIXING LOGIC: This is the "Algorithm" part.
                // We shift bits and XOR them to create a unique fingerprint.
                hash = ((hash << 5) + hash) + static_cast<unsigned char>(buffer[i]);
                hash ^= (hash >> 3); // Extra layer of bit-shuffling
            }
        }

        // Convert the final number into a Hexadecimal string (like "a1b2c3d4")
        std::stringstream ss;
        ss << std::hex << std::setw(8) << std::setfill('0') << hash;
        return ss.str();
    }

    /**
     * Same as above, but for a simple String (used for passwords).
     */
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
