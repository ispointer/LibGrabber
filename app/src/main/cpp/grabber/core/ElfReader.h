
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

/*
#ifndef GRABBER_CORE_ELFREADER_H
#define GRABBER_CORE_ELFREADER_H

#include <string>
#include <map>
#include <set>
#include <elfio/elfio.hpp>
#include "../models/SymbolInfo.h"

class ElfReader {
public:
    struct ReadResult {
        std::map<std::vector<std::string>, std::set<FunctionInfo>> freeFunctions;
        std::map<ClassKey, std::set<MethodInfo>> classes;
    };

    static ReadResult readSymbols(const std::string& filePath);
};

#endif*/

#ifndef GRABBER_CORE_ELFREADER_H
#define GRABBER_CORE_ELFREADER_H

#include <string>
#include <map>
#include <set>
#include <vector>
#include <elfio/elfio.hpp>
#include "../models/SymbolInfo.h"

class ElfReader {
public:
    struct ReadResult {
        std::map<std::vector<std::string>, std::set<FunctionInfo>> freeFunctions;
        std::map<ClassKey, std::set<MethodInfo>> classes;
       // std::map<std::string, ClassInfo> classInfos;
    };

    static ReadResult readSymbols(const std::string& filePath);
};

#endif