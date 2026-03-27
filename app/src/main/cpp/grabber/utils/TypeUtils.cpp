
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


#include "TypeUtils.h"
#include "StringUtils.h"
#include <vector>
#include <algorithm>
#include <cctype>
#include <regex>
#include <set>


std::string TypeUtils::simplifyTypes(std::string s) {
    const std::string ndk_ns = "std::__ndk1::";
    const std::string cxx11_ns = "std::__cxx11::";
    size_t pos;

    try {
        while ((pos = s.find(ndk_ns)) != std::string::npos)
            s.replace(pos, ndk_ns.size(), "std::");
        while ((pos = s.find(cxx11_ns)) != std::string::npos)
            s.replace(pos, cxx11_ns.size(), "std::");

        const std::vector<std::pair<std::string, std::string>> reps = {
                {"std::basic_string<char, std::char_traits<char>, std::allocator<char> >", "std::string"},
                {"std::basic_string<char, std::char_traits<char>, std::allocator<char>>", "std::string"},
                {"std::basic_ostream<char, std::char_traits<char> >", "std::ostream"},
                {"std::basic_ostream<char, std::char_traits<char>>", "std::ostream"},
                {"std::basic_istream<char, std::char_traits<char> >", "std::istream"},
                {"std::basic_istream<char, std::char_traits<char>>", "std::istream"},
                {"std::basic_iostream<char, std::char_traits<char> >", "std::iostream"},
                {"std::basic_iostream<char, std::char_traits<char>>", "std::iostream"},
                {", std::allocator<", "<"},
                {", std::less<", "<"},
                {", std::hash<", "<"},
                {", std::equal_to<", "<"},
                {"std::vector<std::string const<std::string const>", "std::vector<std::string const"},
                {"std::vector<std::string<std::string >", "std::vector<std::string"},
                {"std::unordered_map<std::string, unsigned int, std::hash<std::string>, std::equal_to<std::string><std::pair<std::string const, unsigned int>>",
                                                                                           "std::unordered_map<std::string, unsigned int>"},
        };

        for (const auto& [from, to] : reps) {
            while ((pos = s.find(from)) != std::string::npos)
                s.replace(pos, from.size(), to);
        }
        while ((pos = s.find("const<std::")) != std::string::npos) {
            if (pos > 0) {
                size_t start = s.rfind(' ', pos - 1);
                if (start != std::string::npos && start < pos) {
                    s.erase(start + 1, pos - start);
                } else {
                    s.erase(0, pos + 5);
                }
            } else {
                s.erase(0, pos + 5);
            }
        }

        for (size_t i = 0; i + 1 < s.size(); ++i) {
            if (s[i] == ',' && i + 1 < s.size() && s[i + 1] != ' ') {
                s.insert(i + 1, " ");
                i++;
            }
        }

        for (int pass = 0; pass < 3; ++pass) {
            while ((pos = s.find("> >")) != std::string::npos && pos + 2 < s.size())
                s.replace(pos, 3, ">>");
            while ((pos = s.find(" > >")) != std::string::npos && pos + 3 < s.size())
                s.replace(pos, 4, ">>");
            while ((pos = s.find(">  >")) != std::string::npos && pos + 3 < s.size())
                s.replace(pos, 4, ">>");
        }

        s.erase(std::unique(s.begin(), s.end(),
                            [](char a, char b){ return std::isspace((unsigned char)a) && std::isspace((unsigned char)b); }),
                s.end());

        while ((pos = s.find("std::string const")) != std::string::npos) {
            if (pos + 17 <= s.size()) {
                if (pos + 17 < s.size() && (s[pos + 17] == '>' || s[pos + 17] == ',' || s[pos + 17] == '&')) {
                    s.replace(pos, 17, "std::string");
                } else if (pos + 17 == s.size()) {
                    s.replace(pos, 17, "std::string");
                } else {
                    break;
                }
            } else {
                break;
            }
        }

        StringUtils::trim(s);
        return s;

    } catch (const std::exception& e) {
        return s;
    }
}

std::string TypeUtils::normalizeLambdaName(const std::string& tok) {
    if (tok.find("lambda") != std::string::npos) return "lambda";
    if (tok.find("operator()") != std::string::npos) return "lambda::operator()";
    if (tok.find("<lambda") != std::string::npos || tok.find("anonymous") != std::string::npos)
        return "lambda";
    return tok;
}

std::string TypeUtils::normalizeOperator(const std::string& tok) {
    if (tok.rfind("operator",0)==0 || tok.find("operator ")!=std::string::npos) {
        static const std::regex opjunk(R"((operator[^\(\s\<]+)\<[^>]*\>)");
        return std::regex_replace(tok, opjunk, "$1");
    }
    return tok;
}

bool TypeUtils::isCommonSTLContainer(const std::string& tok) {
    static const std::set<std::string> stl = {
            "vector","map","unordered_map","set","unordered_set","list",
            "deque","string","pair","tuple","basic_string"
    };
    size_t lt = tok.find('<');
    std::string base = (lt == std::string::npos) ? tok : tok.substr(0, lt);
    return stl.count(base) > 0;
}

std::string TypeUtils::collapseBadNestedTemplates(std::string s) {
    bool changed = true;
    while (changed) {
        changed = false;
        std::smatch m;
        static const std::regex r(R"(([\w:]+)<\s*([\w:<>]+)<\s*([\w:<>]+)\s*>\s*>\s*)");
        if (std::regex_search(s, m, r)) {
            if (m.size() >= 4) {
                s = std::regex_replace(s, r, "$1<$2>");
                changed = true;
            }
        }
    }
    return s;
}

std::pair<std::vector<std::string>, std::string>
TypeUtils::promoteSTLContainerIfNeeded(const std::vector<std::string>& nsParts,
                                       const std::string& clsTokRaw) {
    std::string clsTok = collapseBadNestedTemplates(clsTokRaw);
    size_t lt = clsTok.find('<');
    std::string base = (lt==std::string::npos) ? clsTok : clsTok.substr(0, lt);

    if (isCommonSTLContainer(base)) {
        return { {"std"}, clsTok };
    }
    return { nsParts, clsTokRaw };
}