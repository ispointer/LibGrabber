
/*
 * Created by aantik 
 * 3/27/2026 6:21 PM
 *
 *   ⋆    ႔ ႔
 *     ᠸ^ ^ ⸝⸝
 *       |、˜〵
 *      じしˍ,)⁐̤ᐷ
 *
 * Fox Mode 🍺
 */


#include "MemoryLayoutAnalyzer.h"
#include "../utils/StringUtils.h"
#include <algorithm>

std::vector<RuntimeFieldInfo> MemoryLayoutAnalyzer::analyzeVTableOffsets(
        ELFIO::elfio& reader,
        const std::string& className,
        ELFIO::Elf64_Addr vtableAddress) {

    std::vector<RuntimeFieldInfo> fields;

    if (vtableAddress == 0) return fields;

    for (const auto& section : reader.sections) {
        ELFIO::Elf64_Addr startAddr = section->get_address();
        ELFIO::Elf64_Addr endAddr = startAddr + section->get_size();

        if (vtableAddress >= startAddr && vtableAddress < endAddr) {
            size_t offsetInSection = vtableAddress - startAddr;

            if (offsetInSection + 64 < section->get_size()) {
            }
            break;
        }
    }

    return fields;
}

std::vector<RuntimeFieldInfo> MemoryLayoutAnalyzer::findStaticFields(
        ELFIO::elfio& reader,
        const std::string& className) {

    std::vector<RuntimeFieldInfo> fields;

    for (const auto& section : reader.sections) {
        if (section->get_name() != ".data" && section->get_name() != ".bss") continue;

        ELFIO::symbol_section_accessor syms(reader, reader.sections[".symtab"]);
        ELFIO::Elf_Xword n = syms.get_symbols_num();

        for (ELFIO::Elf_Xword i = 0; i < n; ++i) {
            std::string name;
            ELFIO::Elf64_Addr value = 0;
            ELFIO::Elf_Xword size = 0;
            unsigned char bind = 0, type = 0, other = 0;
            ELFIO::Elf_Half shndx = 0;
            syms.get_symbol(i, name, value, size, bind, type, shndx, other);

            if (name.empty()) continue;

            std::string demangled = StringUtils::demangle(name);
            if (demangled.find(className + "::") != std::string::npos) {
                RuntimeFieldInfo field;
                field.name = demangled.substr(demangled.find("::") + 2);
                field.offset = value;
                field.size = size;
                field.type = "static";
                fields.push_back(field);
            }
        }
    }

    return fields;
}