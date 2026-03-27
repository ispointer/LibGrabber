
/*
 * Created by aantik 
 * 3/27/2026 6:21 PM
 *
 *   ⋆    ႔ ႔
 *     ᠸ^ ^ ⸝⸝
 *       |、˜〵
 *      じしˍ,)⁐̤ᐷ
 *
 * Fox Mode 🍺
 */

#ifndef GRABBER_ANALYZER_MEMORYLAYOUTANALYZER_H
#define GRABBER_ANALYZER_MEMORYLAYOUTANALYZER_H

#include <string>
#include <vector>
#include <map>
#include <elfio/elfio.hpp>

struct RuntimeFieldInfo {
    std::string name;
    size_t offset;
    size_t size;
    std::string type;

    RuntimeFieldInfo() : offset(0), size(0) {}
};

class MemoryLayoutAnalyzer {
public:
    static std::vector<RuntimeFieldInfo> analyzeVTableOffsets(
            ELFIO::elfio& reader,
            const std::string& className,
            ELFIO::Elf64_Addr vtableAddress);

    static std::vector<RuntimeFieldInfo> findStaticFields(
            ELFIO::elfio& reader,
            const std::string& className);
};

#endif