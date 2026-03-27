
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


#ifndef GRABBER_PARSER_FUNCTIONPARSER_H
#define GRABBER_PARSER_FUNCTIONPARSER_H

#include <string>

struct ParsedFunction {
    std::string returnType;
    std::string namePart;
    bool hasConst = false;
};

class FunctionParser {
public:
    static size_t findLastSpaceBeforeParen(const std::string& s, size_t paren);
    static ParsedFunction splitReturnAndName(const std::string& dem);
};

#endif
