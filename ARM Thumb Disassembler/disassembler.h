#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <string>
#include <map>
#include <cstdint>

class Disassembler {
public:
    Disassembler();
    std::string disassembleInstruction(uint16_t instruction);

private:
    std::string decodeMovImmediate(uint16_t instruction);
    std::string decodeAluOperation(uint16_t instruction);
    std::string decodeBranchInstruction(uint16_t instruction);
    std::string decodeLoadStoreInstruction(uint16_t instruction);
    std::string decodeMultipleLoadStore(uint16_t instruction);

    std::map<uint8_t, std::string> registerNames;

    std::map<uint8_t, std::string> conditionCodes;

    std::map<uint8_t, std::string> aluOperations;
};

#endif 