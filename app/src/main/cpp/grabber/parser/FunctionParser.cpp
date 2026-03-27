
/*
 * Created by aantik 
 * 3/27/2026 4:28 PM
 *
 *   ⋆    ႔ ႔
 *     ᠸ^ ^ ⸝⸝
 *       |、˜〵
 *      じしˍ,)⁐̤ᐷ
 *
 * Fox Mode 🍺
 */


#include "FunctionParser.h"
#include "../utils/StringUtils.h"
#include <cctype>

size_t FunctionParser::findLastSpaceBeforeParen(const std::string& s, size_t paren) {
    int depth = 0;
    size_t lastSpace = std::string::npos;
    for (size_t i = 0; i < paren; ++i) {
        char c = s[i];
        if (c == '<') depth++;
        else if (c == '>') { if (depth > 0) depth--; }
        else if (depth == 0 && std::isspace((unsigned char)c)) lastSpace = i;
    }
    return lastSpace;
}

ParsedFunction FunctionParser::splitReturnAndName(const std::string& dem) {
    ParsedFunction out;
    size_t paren = dem.find('(');
    if (paren == std::string::npos) {
        out.namePart = dem;
        return out;
    }

    size_t closeParen = dem.find(')', paren);
    if (closeParen != std::string::npos) {
        std::string tail = dem.substr(closeParen + 1);
        if (tail.find(" const") != std::string::npos) out.hasConst = true;
    }

    size_t lastSpace = findLastSpaceBeforeParen(dem, paren);
    if (lastSpace == std::string::npos) {
        out.namePart = dem;
    } else {
        out.returnType = dem.substr(0, lastSpace);
        out.namePart = dem.substr(lastSpace + 1);
    }
    StringUtils::trim(out.returnType);
    StringUtils::trim(out.namePart);
    return out;
}