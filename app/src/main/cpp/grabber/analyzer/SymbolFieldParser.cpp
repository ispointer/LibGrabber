
/*
 * Created by aantik 
 * 3/27/2026 6:20 PM
 *
 *   ⋆    ႔ ႔
 *     ᠸ^ ^ ⸝⸝
 *       |、˜〵
 *      じしˍ,)⁐̤ᐷ
 *
 * Fox Mode 🍺
 */

#include "SymbolFieldParser.h"
#include "../utils/StringUtils.h"
#include <regex>
#include <cctype>
#include <algorithm>

std::vector<SymbolFieldInfo> SymbolFieldParser::parseFromSymbols(ELFIO::elfio& reader,
                                                                 const std::string& className) {
    std::vector<SymbolFieldInfo> fields;

    for (const auto& section : reader.sections) {
        if (!section) continue;
        if (section->get_type() != ELFIO::SHT_SYMTAB &&
            section->get_type() != ELFIO::SHT_DYNSYM) continue;

        ELFIO::symbol_section_accessor syms(reader, section.get());
        const ELFIO::Elf_Xword n = syms.get_symbols_num();

        for (ELFIO::Elf_Xword i = 0; i < n; ++i) {
            std::string raw;
            ELFIO::Elf64_Addr value = 0;
            ELFIO::Elf_Xword size = 0;
            unsigned char bind = 0, type = 0, other = 0;
            ELFIO::Elf_Half shndx = 0;
            syms.get_symbol(i, raw, value, size, bind, type, shndx, other);

            if (raw.empty()) continue;
            if (type != ELFIO::STT_OBJECT) continue;

            std::string demangled = StringUtils::demangle(raw);
            if (demangled.empty()) continue;

            std::string searchPattern = className + "::";
            size_t pos = demangled.find(searchPattern);
            if (pos != std::string::npos) {
                SymbolFieldInfo field;
                field.mangledName = raw;
                field.name = demangled.substr(pos + searchPattern.length());
                size_t ltPos = field.name.find('<');
                if (ltPos != std::string::npos) {
                    field.name = field.name.substr(0, ltPos);
                }
                if (raw.find("Ii") != std::string::npos ||
                    raw.find("i") != std::string::npos ||
                    raw.find("j") != std::string::npos) {
                    field.type = "int";
                } else if (raw.find("f") != std::string::npos) {
                    field.type = "float";
                } else if (raw.find("d") != std::string::npos) {
                    field.type = "double";
                } else if (raw.find("P") != std::string::npos) {
                    field.type = "void*";
                } else if (raw.find("NSt7__cxx1112basic_string") != std::string::npos ||
                           raw.find("Ss") != std::string::npos) {
                    field.type = "std::string";
                } else if (raw.find("St6vector") != std::string::npos) {
                    field.type = "std::vector";
                } else {
                    field.type = "unknown";
                }

                if (shndx != ELFIO::SHN_UNDEF && value > 0 && value < 0x1000) {
                    field.offset = (size_t)value;
                } else {
                    field.offset = 0;
                }

                fields.push_back(field);
            }
        }
    }

    std::sort(fields.begin(), fields.end(),
              [](const SymbolFieldInfo& a, const SymbolFieldInfo& b) {
                  return a.offset < b.offset;
              });

    fields.erase(std::unique(fields.begin(), fields.end(),
                             [](const SymbolFieldInfo& a, const SymbolFieldInfo& b) {
                                 return a.name == b.name && a.offset == b.offset;
                             }), fields.end());

    return fields;
}