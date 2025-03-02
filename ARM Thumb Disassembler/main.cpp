#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <map>
#include <string>
#include "disassembler.h"

void printUsage() {
    std::cout << "ARM Thumb Disassembler" << std::endl;
    std::cout << "Usage: disassembler <input_file> [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -o <output_file>   Write output to file instead of stdout" << std::endl;
    std::cout << "  -v                 Verbose mode (show instruction bytes)" << std::endl;
    std::cout << "  -h                 Display this help message" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    std::string inputFileName;
    std::string outputFileName = "";
    bool verboseMode = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-h") {
            printUsage();
            return 0;
        }
        else if (arg == "-v") {
            verboseMode = true;
        }
        else if (arg == "-o" && i + 1 < argc) {
            outputFileName = argv[++i];
        }
        else if (inputFileName.empty()) {
            inputFileName = arg;
        }
    }

    std::ifstream file(inputFileName, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not open file " << inputFileName << std::endl;
        return 1;
    }

    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(file), {});
    file.close();

    if (buffer.size() % 2 != 0) {
        std::cerr << "Warning: File size is not a multiple of 2 bytes. Padding with 0." << std::endl;
        buffer.push_back(0);
    }

    Disassembler disasm;

    std::ofstream outFile;
    std::streambuf* originalCoutBuffer = nullptr;

    if (!outputFileName.empty()) {
        outFile.open(outputFileName);
        if (!outFile) {
            std::cerr << "Error: Could not open output file " << outputFileName << std::endl;
            return 1;
        }
        originalCoutBuffer = std::cout.rdbuf();
        std::cout.rdbuf(outFile.rdbuf());
    }

    std::cout << "Address    " << (verboseMode ? "Bytes      " : "") << "Instruction" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

    for (size_t i = 0; i < buffer.size(); i += 2) {
        uint16_t instruction = (buffer[i + 1] << 8) | buffer[i]; 
        std::string disassembled = disasm.disassembleInstruction(instruction);

        std::cout << std::hex << std::setw(8) << std::setfill('0') << i << "  ";

        if (verboseMode) {
            std::cout << std::hex << std::setw(4) << std::setfill('0') << instruction << "      ";
        }

        std::cout << disassembled << std::endl;
    }

    if (originalCoutBuffer) {
        std::cout.rdbuf(originalCoutBuffer);
        outFile.close();
        std::cout << "Disassembly written to " << outputFileName << std::endl;
    }

    return 0;
}