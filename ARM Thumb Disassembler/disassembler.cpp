#include "disassembler.h"
#include <sstream>
#include <iomanip>

Disassembler::Disassembler() {
    registerNames[0] = "r0";
    registerNames[1] = "r1";
    registerNames[2] = "r2";
    registerNames[3] = "r3";
    registerNames[4] = "r4";
    registerNames[5] = "r5";
    registerNames[6] = "r6";
    registerNames[7] = "r7";
    registerNames[8] = "r8";
    registerNames[9] = "r9";
    registerNames[10] = "r10";
    registerNames[11] = "r11";
    registerNames[12] = "r12";
    registerNames[13] = "sp";  
    registerNames[14] = "lr";  
    registerNames[15] = "pc";  

    conditionCodes[0] = "eq";  
    conditionCodes[1] = "ne";  
    conditionCodes[2] = "cs";  
    conditionCodes[3] = "cc";  
    conditionCodes[4] = "mi";  
    conditionCodes[5] = "pl";  
    conditionCodes[6] = "vs";  
    conditionCodes[7] = "vc";  
    conditionCodes[8] = "hi";  
    conditionCodes[9] = "ls";  
    conditionCodes[10] = "ge"; 
    conditionCodes[11] = "lt"; 
    conditionCodes[12] = "gt"; 
    conditionCodes[13] = "le"; 
    conditionCodes[14] = "";   

    aluOperations[0] = "and";
    aluOperations[1] = "eor";
    aluOperations[2] = "lsl";
    aluOperations[3] = "lsr";
    aluOperations[4] = "asr";
    aluOperations[5] = "adc";
    aluOperations[6] = "sbc";
    aluOperations[7] = "ror";
    aluOperations[8] = "tst";
    aluOperations[9] = "neg";
    aluOperations[10] = "cmp";
    aluOperations[11] = "cmn";
    aluOperations[12] = "orr";
    aluOperations[13] = "mul";
    aluOperations[14] = "bic";
    aluOperations[15] = "mvn";
}

std::string Disassembler::disassembleInstruction(uint16_t instruction) {

    if ((instruction & 0xF800) == 0x2000) {
        return decodeMovImmediate(instruction);
    }

    else if ((instruction & 0xFC00) == 0x4000) {
        return decodeAluOperation(instruction);
    }

    else if ((instruction & 0xF000) == 0x6000 ||
        (instruction & 0xF000) == 0x7000 ||
        (instruction & 0xF000) == 0x8000 ||
        (instruction & 0xF000) == 0x9000) {
        return decodeLoadStoreInstruction(instruction);
    }

    else if ((instruction & 0xF000) == 0xD000) {
        return decodeBranchInstruction(instruction);
    }

    else if ((instruction & 0xF000) == 0xC000) {
        return decodeMultipleLoadStore(instruction);
    }

    std::stringstream ss;
    ss << ".word 0x" << std::hex << std::setw(4) << std::setfill('0') << instruction;
    return ss.str();
}

std::string Disassembler::decodeMovImmediate(uint16_t instruction) {
    uint8_t rd = (instruction >> 8) & 0x7;
    uint8_t imm = instruction & 0xFF;

    std::stringstream ss;
    ss << "mov " << registerNames[rd] << ", #" << static_cast<int>(imm);
    return ss.str();
}

std::string Disassembler::decodeAluOperation(uint16_t instruction) {
    uint8_t opcode = (instruction >> 6) & 0xF;
    uint8_t rs = (instruction >> 3) & 0x7;
    uint8_t rd = instruction & 0x7;

    std::stringstream ss;

    if (opcode == 8 || opcode == 10 || opcode == 11) { 
        ss << aluOperations[opcode] << " " << registerNames[rd] << ", " << registerNames[rs];
    }
    else {
        ss << aluOperations[opcode] << " " << registerNames[rd] << ", " << registerNames[rd] << ", " << registerNames[rs];
    }

    return ss.str();
}

std::string Disassembler::decodeBranchInstruction(uint16_t instruction) {
    uint8_t cond = (instruction >> 8) & 0xF;
    int8_t offset = static_cast<int8_t>(instruction & 0xFF); 

    std::stringstream ss;
    if (cond == 0xE) { 
        ss << "b ";
    }
    else {
        ss << "b" << conditionCodes[cond] << " ";
    }

    ss << "PC + " << (offset * 2 + 4);

    return ss.str();
}

std::string Disassembler::decodeLoadStoreInstruction(uint16_t instruction) {
    bool isLoad = (instruction & 0x0800) != 0;
    uint8_t rd = instruction & 0x7;

    std::stringstream ss;

    if ((instruction & 0xF000) == 0x6000) {
        uint8_t rb = (instruction >> 3) & 0x7;
        uint8_t imm5 = (instruction >> 6) & 0x1F;

        ss << (isLoad ? "ldr " : "str ") << registerNames[rd] << ", [" << registerNames[rb];
        if (imm5 != 0) {
            ss << ", #" << (imm5 * 4);
        }
        ss << "]";
    }
    else if ((instruction & 0xF000) == 0x7000) {
        uint8_t rb = (instruction >> 3) & 0x7;
        uint8_t ro = (instruction >> 6) & 0x7;

        ss << (isLoad ? "ldr " : "str ") << registerNames[rd] << ", [" << registerNames[rb];
        ss << ", " << registerNames[ro] << "]";
    }
    else {
        bool isSP = (instruction & 0xF000) == 0x9000;
        uint8_t imm8 = instruction & 0xFF;

        ss << (isLoad ? "ldr " : "str ") << registerNames[rd] << ", [";
        ss << (isSP ? "sp" : "pc") << ", #" << (imm8 * 4) << "]";
    }

    return ss.str();
}

std::string Disassembler::decodeMultipleLoadStore(uint16_t instruction) {
    bool isLoad = (instruction & 0x0800) != 0;
    uint8_t rn = (instruction >> 8) & 0x7;
    uint8_t register_list = instruction & 0xFF;

    std::stringstream ss;
    ss << (isLoad ? "ldmia " : "stmia ") << registerNames[rn] << "!, {";

    bool firstReg = true;
    for (int i = 0; i < 8; i++) {
        if (register_list & (1 << i)) {
            if (!firstReg) {
                ss << ", ";
            }
            ss << registerNames[i];
            firstReg = false;
        }
    }

    ss << "}";
    return ss.str();
}