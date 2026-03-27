
/*
 * Created by aantik 
 * 3/27/2026 4:29 PM
 *
 *   ⋆    ႔ ႔
 *     ᠸ^ ^ ⸝⸝
 *       |、˜〵
 *      じしˍ,)⁐̤ᐷ
 *
 * Fox Mode 🍺
 */


#include "ElfReader.h"
#include "SymbolProcessor.h"
#include <iostream>

using namespace ELFIO;

ElfReader::ReadResult ElfReader::readSymbols(const std::string& filePath) {
    ReadResult result;
    elfio reader;

    if (!reader.load(filePath)) {return result;}

    for (const auto& section : reader.sections) {
        if (!section) continue;
        if (section->get_type() != SHT_SYMTAB && section->get_type() != SHT_DYNSYM) continue;

        symbol_section_accessor syms(reader, section.get());
        const Elf_Xword n = syms.get_symbols_num();

        for (Elf_Xword i = 0; i < n; ++i) {
            std::string raw;
            Elf64_Addr value = 0;
            Elf_Xword size = 0;
            unsigned char bind = 0, type = 0, other = 0;
            Elf_Half shndx = 0;
            syms.get_symbol(i, raw, value, size, bind, type, shndx, other);

            if (raw.empty()) continue;
            if (type != STT_FUNC) continue;
            if (shndx == SHN_UNDEF || value == 0) continue;

            auto processed = SymbolProcessor::processSymbol(raw, value);

            for (const auto& [ns, funcs] : processed.freeFunctions) {
                result.freeFunctions[ns].insert(funcs.begin(), funcs.end());
            }
            for (const auto& [key, methods] : processed.classes) {
                result.classes[key].insert(methods.begin(), methods.end());
            }
        }
    }

    return result;
}