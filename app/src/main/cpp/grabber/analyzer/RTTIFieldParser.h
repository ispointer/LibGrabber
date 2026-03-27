
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

#ifndef GRABBER_ANALYZER_RTTIFIELDPARSER_H
#define GRABBER_ANALYZER_RTTIFIELDPARSER_H

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <elfio/elfio.hpp>

struct RawFieldInfo {
    std::string name;
    std::string typeName;
    size_t offset;
    size_t size;
    bool isPublic;

    RawFieldInfo() : offset(0), size(0), isPublic(true) {}
};

class RTTIFieldParser {
public:
    static std::map<std::string, std::vector<RawFieldInfo>> parseAllFields(ELFIO::elfio& reader);

private:
    struct RTTICompleteObjectLocator {
        uint32_t signature;
        uint32_t offset;
        uint32_t cdOffset;
        uint64_t typeDescriptor;
        uint64_t classDescriptor;
    };

    struct RTTIClassDescriptor {
        uint64_t typeInfo;
        uint32_t numBaseClasses;
        uint32_t numFields;
        uint64_t baseClassArray;
        uint64_t fieldArray;
    };

    struct RTTIFieldDescriptor {
        uint64_t name;
        uint32_t offset;
        uint32_t flags;
        uint64_t typeDescriptor;
    };

    static bool isValidPointer(uint64_t ptr, size_t sectionSize);
};

#endif