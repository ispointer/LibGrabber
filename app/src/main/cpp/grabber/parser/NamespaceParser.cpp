
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


#include "NamespaceParser.h"
#include "../utils/StringUtils.h"
#include "../utils/TypeUtils.h"
#include <cctype>

size_t NamespaceParser::findLastColoncolonBefore(const std::string& s, size_t paren) {
    int depth = 0;
    size_t lastCC = std::string::npos;
    for (size_t i = 0; i < paren; ++i) {
        char c = s[i];
        if (c == '<') depth++;
        else if (c == '>') { if (depth > 0) depth--; }
        else if (depth == 0 && c == ':' && i > 0 && s[i - 1] == ':') {
            lastCC = i - 1;
        }
    }
    return lastCC;
}

std::pair<std::vector<std::string>, std::string>
NamespaceParser::splitNamespacesAndClassToken(const std::string& classFull) {
    std::vector<std::string> parts;
    std::string cur;
    int depth = 0;
    for (size_t i = 0; i < classFull.size(); ++i) {
        char c = classFull[i];
        if (c == '<') {
            depth++;
            cur.push_back(c);
        } else if (c == '>') {
            if (depth > 0) --depth;
            cur.push_back(c);
        } else if (c == ':' && i + 1 < classFull.size() && classFull[i + 1] == ':' && depth == 0) {
            StringUtils::trim(cur);
            if (!cur.empty()) parts.push_back(cur);
            cur.clear();
            ++i;
        } else {
            cur.push_back(c);
        }
    }
    StringUtils::trim(cur);
    if (!cur.empty()) parts.push_back(cur);

    if (parts.empty()) {
        return { {}, StringUtils::sanitizeClassWithTemplates(classFull) };
    }

    std::vector<std::string> ns;
    for (size_t i = 0; i + 1 < parts.size(); ++i) {
        std::string p = parts[i];
        StringUtils::trim(p);
        if (!p.empty()) ns.push_back(p);
    }

    std::string clsToken = parts.back();
    StringUtils::trim(clsToken);
    std::string cls = StringUtils::sanitizeClassWithTemplates(clsToken);
    return { ns, cls };
}

bool NamespaceParser::looksLikeNamespaceContainer(const std::string& clsSanitized,
                                                  const std::string& methodSig) {
    if (clsSanitized.empty()) return true;
    if (clsSanitized == "std") return true;

    if (clsSanitized.size() > 2 && clsSanitized.substr(clsSanitized.size() - 2) == "_t")
        return false;

    bool hasAlpha = false, anyUpper = false;
    for (char c : clsSanitized) {
        if (std::isalpha((unsigned char)c)) {
            hasAlpha = true;
            if (std::isupper((unsigned char)c)) anyUpper = true;
        }
    }
    if (hasAlpha && !anyUpper) {
        std::string name = methodSig;
        size_t p = name.find('(');
        if (p != std::string::npos) name = name.substr(0, p);
        size_t lt = name.find('<');
        if (lt != std::string::npos) name = name.substr(0, lt);
        StringUtils::trim(name);
        if (name == clsSanitized || (name.size() > 1 && name[0] == '~' && name.substr(1) == clsSanitized))
            return false;
        return true;
    }
    return false;
}