
/*
 * Created by aantik 
 * 3/27/2026 4:27 PM
 *
 *   ⋆    ႔ ႔
 *     ᠸ^ ^ ⸝⸝
 *       |、˜〵
 *      じしˍ,)⁐̤ᐷ
 *
 * Fox Mode 🍺
 */


#ifndef GRABBER_UTILS_STRINGUTILS_H
#define GRABBER_UTILS_STRINGUTILS_H

#include <string>
#include <vector>

class StringUtils {
public:
    static void trim(std::string& s);
    static void trimRight(std::string& s);
    static std::string demangle(const std::string& name);
    static std::string replaceExtensionWithHPP(const std::string& path);
    static std::string stripNamespaceOne(const std::string& s);
    static std::vector<std::string> splitTemplateArgs(const std::string& body);
    static std::string sanitizeClassWithTemplates(const std::string& full);
    static void stripThunkPrefixes(std::string& s);
    static std::string formatNamespaces(const std::string& input);
    static void tightenAngles(std::string& s);
    static void fixRemainingTemplateIssues(std::string& s);
    static std::string joinNamespace(const std::vector<std::string>& ns);
};

#endif