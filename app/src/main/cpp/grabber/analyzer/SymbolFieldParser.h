
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

#ifndef GRABBER_ANALYZER_SYMBOLFIELDPARSER_H
#define GRABBER_ANALYZER_SYMBOLFIELDPARSER_H

#include <string>
#include <vector>
#include <map>
#include <elfio/elfio.hpp>

struct SymbolFieldInfo {
    std::string name;
    std::string mangledName;
    size_t offset;
    std::string type;

    SymbolFieldInfo() : offset(0) {}
};

class SymbolFieldParser {
public:
    static std::vector<SymbolFieldInfo> parseFromSymbols(ELFIO::elfio& reader,
                                                         const std::string& className);
};

#endif