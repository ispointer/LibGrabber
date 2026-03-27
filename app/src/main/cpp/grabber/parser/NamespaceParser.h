
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


#ifndef GRABBER_PARSER_NAMESPACEPARSER_H
#define GRABBER_PARSER_NAMESPACEPARSER_H

#include <string>
#include <vector>
#include <utility>

class NamespaceParser {
public:
    static size_t findLastColoncolonBefore(const std::string& s, size_t paren);
    static std::pair<std::vector<std::string>, std::string>
    splitNamespacesAndClassToken(const std::string& classFull);
    static bool looksLikeNamespaceContainer(const std::string& clsSanitized,
                                            const std::string& methodSig);
};

#endif
