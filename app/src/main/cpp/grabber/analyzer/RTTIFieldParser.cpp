
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


#include "RTTIFieldParser.h"
#include <cstring>
#include <algorithm>

bool RTTIFieldParser::isValidPointer(uint64_t ptr, size_t sectionSize) {
    return ptr > 0 && ptr < sectionSize - 8;
}

std::map<std::string, std::vector<RawFieldInfo>>
RTTIFieldParser::parseAllFields(ELFIO::elfio& reader) {

    std::map<std::string, std::vector<RawFieldInfo>> result;

    for (const auto& section : reader.sections) {
        std::string sectionName = section->get_name();

        if (sectionName != ".rodata" && sectionName != ".data.rel.ro") {
            continue;
        }

        const char* data = section->get_data();
        size_t sectionSize = section->get_size();

        if (!data || sectionSize < 64) continue;

        for (size_t i = 0; i + 32 < sectionSize; i += 8) {
            uint32_t* sigPtr = (uint32_t*)(data + i);
            if (*sigPtr != 0x01) continue;

            uint64_t classDescOffset = *(uint64_t*)(data + i + 16);
            if (!isValidPointer(classDescOffset, sectionSize)) continue;

            const char* classDesc = data + classDescOffset;

            uint64_t typeInfoOffset = *(uint64_t*)(classDesc);
            if (!isValidPointer(typeInfoOffset, sectionSize)) continue;

            const char* typeInfo = data + typeInfoOffset;
            const char* className = (const char*)(typeInfo + 8);

            if (!className || strlen(className) == 0) continue;

            uint32_t fieldCount = *(uint32_t*)(classDesc + 24);
            if (fieldCount == 0 || fieldCount > 1000) continue;

            uint64_t fieldArrayOffset = *(uint64_t*)(classDesc + 32);
            if (!isValidPointer(fieldArrayOffset, sectionSize)) continue;

            const char* fieldArray = data + fieldArrayOffset;
            std::vector<RawFieldInfo> fields;

            for (uint32_t f = 0; f < fieldCount; f++) {
                const char* fieldDesc = fieldArray + (f * 24);
                if (fieldDesc + 24 > data + sectionSize) break;

                uint64_t nameOffset = *(uint64_t*)(fieldDesc);
                if (!isValidPointer(nameOffset, sectionSize)) continue;

                const char* fieldName = (const char*)(data + nameOffset);
                if (!fieldName) continue;

                uint32_t fieldOffset = *(uint32_t*)(fieldDesc + 8);

                uint64_t typeInfoOffset2 = *(uint64_t*)(fieldDesc + 16);
                const char* typeName = "unknown";
                if (isValidPointer(typeInfoOffset2, sectionSize)) {
                    typeName = (const char*)(data + typeInfoOffset2 + 8);
                }

                uint32_t flags = *(uint32_t*)(fieldDesc + 12);

                RawFieldInfo field;
                field.name = fieldName;
                field.typeName = typeName;
                field.offset = fieldOffset;
                field.size = 0;
                field.isPublic = (flags & 0x01) != 0;

                fields.push_back(field);
            }

            if (!fields.empty()) {
                std::string cleanClassName = className;
                while (!cleanClassName.empty() && (isdigit(cleanClassName[0]) || cleanClassName[0] == '*')) {
                    cleanClassName.erase(0, 1);
                }
                result[cleanClassName] = fields;
            }
        }
    }

    return result;
}