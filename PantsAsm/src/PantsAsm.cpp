#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "PantsIsa.hpp"
#include "fmt/format.h"

using namespace pants;
void emit(std::vector<char> &buf, uint32_t i) {
    buf.push_back((char)i);
    buf.push_back((char)i >> 8);
    buf.push_back((char)i >> 16);
    buf.push_back((char)i >> 24);
}

void emit(std::vector<char> &buf, uint16_t i) {
    buf.push_back((char)i);
    buf.push_back((char)i >> 8);
}

Opcode getOpcode(const std::string &mnemonic) {
    if (!g_opcode_names.count(mnemonic)) {
        throw std::runtime_error(fmt::format("Bad opcode: {}", mnemonic));
    }

    auto opcode = g_opcode_names[mnemonic];

    return opcode;
}

Register emitReg(std::vector<char> &buf, std::istream &is) {
    std::string name;
    is >> name;

    try {
        auto reg = RegisterFromName(name);
        emit(buf, static_cast<uint16_t>(reg));
        return reg;
    }
    catch (...) {
        throw std::runtime_error(fmt::format("Bad register: {}", name));
    }
}

using Labels = std::unordered_map<std::string, uint64_t>;
using LabelFixups = std::unordered_map<std::string, uint64_t>;

uint32_t emitU32(std::vector<char> &buf, std::istream &is,
                 LabelFixups &fixups) {
    std::string s;
    is >> s;

    if (s[0] == '.') {
        fixups[s] = buf.size();
        emit(buf, uint32_t{0});
        return 0;
    }

    uint32_t u = stoi(s);
    emit(buf, u);
    return u;
}

bool isLabel(std::string &str) { return str.back() == ':'; }

void applyFixups(std::vector<char> &code, const Labels &labels,
                 const LabelFixups &fixups) {
    for (auto fixup : fixups) {
        auto addr = fixup.second;
        std::string label{fixup.first, 1};

        auto val = uint32_t(labels.at(label));
        code[addr] = ((char)val);
        code[addr + 1] = ((char)val >> 8);
        code[addr + 2] = ((char)val >> 16);
        code[addr + 3] = ((char)val >> 24);
    }
}

void assemble(std::istream &is) {
    Labels labels;
    LabelFixups label_fixups;

    uint64_t addr = 0;
    std::vector<char> code{};

    while (true) {
        std::string text;
        is >> text;
        if (!is) {
            applyFixups(code, labels, label_fixups);
            for (auto c : code)
                std::cout << c;
            return;
        }

        if (isLabel(text)) {
            labels[std::string{text, 0, text.length() - 1}] = addr;
        } else {
            auto op = getOpcode(text);
            emit(code, static_cast<uint16_t>(op));

            switch (op) {
            case Opcode::add_:
            case Opcode::sub_:
            case Opcode::mul_:
            case Opcode::div_:
            case Opcode::and_:
            case Opcode::or_:
            case Opcode::compare_:
                emitReg(code, is);
                emitReg(code, is);
                emitReg(code, is);
                break;

            case Opcode::copy_:
                emitReg(code, is);
                emitReg(code, is);
                emit(code, uint16_t{0});
                break;

            case Opcode::not_:
            case Opcode::load_:
            case Opcode::store_:
            case Opcode::branch_:
                emit(code, uint16_t{0});
                emitReg(code, is);
                emit(code, uint16_t{0});
                break;

            case Opcode::set_:
                emitReg(code, is);
                emitU32(code, is, label_fixups);
                break;

            case Opcode::jump_:
            case Opcode::halt_:
                emit(code, uint16_t{0});
                emit(code, uint32_t{0});
                break;

            default:
                std::cerr << "Unhandled opcode " << static_cast<uint16_t>(op);
                abort();
                break;
            }

            addr += 8;
        }
    }
}

int main() {
    std::istream &is = std::cin;

    assemble(is);
}
