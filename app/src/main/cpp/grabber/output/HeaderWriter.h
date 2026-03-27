
/*
 * Created by aantik 
 * 3/27/2026 4:30 PM
 *
 *   ⋆    ႔ ႔
 *     ᠸ^ ^ ⸝⸝
 *       |、˜〵
 *      じしˍ,)⁐̤ᐷ
 *
 * Fox Mode 🍺
 */


#ifndef GRABBER_OUTPUT_HEADERWRITER_H
#define GRABBER_OUTPUT_HEADERWRITER_H

#include <ostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include "../models/SymbolInfo.h"

class HeaderWriter {
public:
    static void write(std::ostream& out,
                      const std::map<std::vector<std::string>, std::set<FunctionInfo>>& freeFunctions,
                      const std::map<ClassKey, std::set<MethodInfo>>& classes);
};

#endif