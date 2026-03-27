
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

#include "DWARFFieldParser.h"
#include "../utils/StringUtils.h"

std::map<std::string, std::vector<DwarfFieldInfo>>
DWARFFieldParser::parseAllFields(const std::string& filePath) {
    std::map<std::string, std::vector<DwarfFieldInfo>> result;
    ELFIO::elfio reader;
    if (!reader.load(filePath)) {
    return result;
    }
    bool hasDebugInfo = false;
    for (const auto& section : reader.sections) {
        std::string name = section->get_name();
        if (name == ".debug_info") {
         hasDebugInfo = true;
         break;
        }
    }
    if (!hasDebugInfo) {
    return result;
    }

    return result;
}

bool DWARFFieldParser::hasDWARF(const std::string& filePath) {
    ELFIO::elfio reader;
    if (!reader.load(filePath)) {
    return false;
    }
    for (const auto& section : reader.sections) {
        std::string name = section->get_name();
        if (name == ".debug_info" || name == ".debug_abbrev" ||
            name == ".debug_line" || name == ".debug_str") {
            return true;
        }
    }
    return false;
}