
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

#ifndef GRABBER_ANALYZER_DWARFFIELDPARSER_H
#define GRABBER_ANALYZER_DWARFFIELDPARSER_H

#include <string>
#include <vector>
#include <map>
#include <elfio/elfio.hpp>

struct DwarfFieldInfo {
    std::string name;
    std::string type;
    size_t offset;
    size_t size;
    bool isPrivate;
    bool isStatic;

    DwarfFieldInfo() : offset(0), size(0), isPrivate(false), isStatic(false) {}
};

class DWARFFieldParser {
public:
    static std::map<std::string, std::vector<DwarfFieldInfo>> parseAllFields(const std::string& filePath);
    static bool hasDWARF(const std::string& filePath);
};

#endif