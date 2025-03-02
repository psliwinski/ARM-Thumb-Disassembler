#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

int main() {
    std::vector<uint16_t> instructions = {
        0x2001,  
        0x2102,  
        0x1840,  
        0x4770,  
        0x6801,  
        0x6042,  
        0xB510,  
        0xBD10,  
        0xD001,  
        0xE003   
    };

    system("mkdir -p test_files");

    std::ofstream outFile("test_files/simple.bin", std::ios::binary);
    if (!outFile) {
        std::cerr << "Error: Could not create test file" << std::endl;
        return 1;
    }

    for (uint16_t instr : instructions) {
        uint8_t low = instr & 0xFF;
        uint8_t high = (instr >> 8) & 0xFF;
        outFile.write(reinterpret_cast<const char*>(&low), 1);
        outFile.write(reinterpret_cast<const char*>(&high), 1);
    }

    outFile.close();
    std::cout << "Test binary file created at test_files/simple.bin" << std::endl;

    return 0;
}