
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


#include "SymbolProcessor.h"
#include "../utils/StringUtils.h"
#include "../utils/TypeUtils.h"
#include "../parser/FunctionParser.h"
#include "../parser/NamespaceParser.h"

using namespace ELFIO;

SymbolProcessor::ProcessingResult SymbolProcessor::processSymbol(const std::string& symbol,
                                                                 Elf64_Addr address) {
    ProcessingResult result;
    std::string dem = StringUtils::demangle(symbol);
    if (dem.empty()) return result;

    StringUtils::stripThunkPrefixes(dem);
    StringUtils::fixRemainingTemplateIssues(dem);
    StringUtils::tightenAngles(dem);
    dem = TypeUtils::simplifyTypes(dem);
    dem = StringUtils::formatNamespaces(dem);
    if (dem.empty()) return result;

    ParsedFunction pf = FunctionParser::splitReturnAndName(dem);
    size_t paren = pf.namePart.find('(');
    if (paren == std::string::npos) return result;

    size_t lastCC = NamespaceParser::findLastColoncolonBefore(pf.namePart, paren);

    if (lastCC == std::string::npos) {
        std::string funcToken = pf.namePart.substr(0, paren);
        StringUtils::trim(funcToken);
        auto [nsPartsGuess, lastNameTok] = NamespaceParser::splitNamespacesAndClassToken(funcToken);
        std::string finalSig = lastNameTok + pf.namePart.substr(funcToken.size());
        if (pf.hasConst && finalSig.rfind(" const") == std::string::npos)
            finalSig += " const";
        result.freeFunctions[nsPartsGuess].insert({pf.returnType, finalSig, address});
        return result;
    }

    std::string classFull = pf.namePart.substr(0, lastCC);
    StringUtils::trim(classFull);
    std::string methodWithParams = pf.namePart.substr(lastCC + 2);
    StringUtils::trim(methodWithParams);

    auto [nsParts0, clsSanitized0] = NamespaceParser::splitNamespacesAndClassToken(classFull);
    std::string clsSanitizedProcessed = TypeUtils::normalizeLambdaName(clsSanitized0);
    clsSanitizedProcessed = TypeUtils::normalizeOperator(clsSanitizedProcessed);
    auto promoted = TypeUtils::promoteSTLContainerIfNeeded(nsParts0, clsSanitizedProcessed);
    auto nsParts = promoted.first;
    auto clsSanitized = promoted.second;

    if (!pf.returnType.empty()) {
        std::string rt = pf.returnType;
        StringUtils::trim(rt);
        while (!rt.empty() && (rt.back() == '*' || rt.back() == '&' ||
                               std::isspace((unsigned char)rt.back()))) rt.pop_back();

        size_t rt_col = rt.find("::");
        if (rt_col != std::string::npos) {
            std::string rt_top = rt.substr(0, rt_col);
            StringUtils::trim(rt_top);
            std::string clsCompare = clsSanitized;
            size_t ltpos = clsCompare.find('<');
            if (ltpos != std::string::npos) clsCompare = clsCompare.substr(0, ltpos);
            StringUtils::trim(clsCompare);

            if (!rt_top.empty() && rt_top == clsCompare) {
                std::vector<std::string> newNs = nsParts;
                newNs.push_back(rt_top);
                size_t lp = methodWithParams.find('(');
                std::string funcNameTok = (lp == std::string::npos) ?
                                          methodWithParams : methodWithParams.substr(0, lp);
                std::string finalSig = funcNameTok + methodWithParams.substr(funcNameTok.size());
                if (pf.hasConst && finalSig.rfind(" const") == std::string::npos)
                    finalSig += " const";
                result.freeFunctions[newNs].insert({pf.returnType, finalSig, address});
                return result;
            }
        }
    }

    if (NamespaceParser::looksLikeNamespaceContainer(clsSanitized, methodWithParams)) {
        nsParts.push_back(clsSanitized);
        size_t lp = methodWithParams.find('(');
        std::string funcNameTok = (lp == std::string::npos) ?
                                  methodWithParams : methodWithParams.substr(0, lp);
        std::string finalSig = funcNameTok + methodWithParams.substr(funcNameTok.size());
        if (pf.hasConst && finalSig.rfind(" const") == std::string::npos)
            finalSig += " const";
        result.freeFunctions[nsParts].insert({pf.returnType, finalSig, address});
        return result;
    }

    std::string finalSig = methodWithParams;
    if (pf.hasConst && finalSig.rfind(" const") == std::string::npos)
        finalSig += " const";

    {
        std::string n = methodWithParams;
        size_t p = n.find('(');
        if (p != std::string::npos) n = n.substr(0, p);
        size_t lt = n.find('<');
        if (lt != std::string::npos) n = n.substr(0, lt);
        StringUtils::trim(n);
        if (n == clsSanitized || (n.size() > 1 && n[0] == '~' && n.substr(1) == clsSanitized)) {
            pf.returnType.clear();
        }
    }

    ClassKey key{ nsParts, clsSanitized };
    result.classes[key].insert({pf.returnType, finalSig, address});
    return result;
}