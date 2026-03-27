
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


#ifndef GRABBER_UTILS_TYPEUTILS_H
#define GRABBER_UTILS_TYPEUTILS_H

#include <string>
#include <vector>

class TypeUtils {
public:
    static std::string simplifyTypes(std::string s);
    static std::string normalizeLambdaName(const std::string& tok);
    static std::string normalizeOperator(const std::string& tok);
    static bool isCommonSTLContainer(const std::string& tok);
    static std::string collapseBadNestedTemplates(std::string s);
    static std::pair<std::vector<std::string>, std::string>
    promoteSTLContainerIfNeeded(const std::vector<std::string>& nsParts,
                                const std::string& clsTokRaw);
};

#endif