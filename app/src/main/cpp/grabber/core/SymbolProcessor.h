
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

#ifndef GRABBER_CORE_SYMBOLPROCESSOR_H
#define GRABBER_CORE_SYMBOLPROCESSOR_H

#include <string>
#include <map>
#include <set>
#include <vector>
#include <elfio/elfio.hpp>
#include "../models/SymbolInfo.h"

class SymbolProcessor {
public:
    struct ProcessingResult {
        std::map<std::vector<std::string>, std::set<FunctionInfo>> freeFunctions;
        std::map<ClassKey, std::set<MethodInfo>> classes;
    };

    static ProcessingResult processSymbol(const std::string& symbol, ELFIO::Elf64_Addr address);
};

#endif