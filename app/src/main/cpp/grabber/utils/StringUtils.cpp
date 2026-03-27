
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


#include "StringUtils.h"
#include <cxxabi.h>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <sstream>

void StringUtils::trimRight(std::string& s) {
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back())))
        s.pop_back();
}

void StringUtils::trim(std::string& s) {
    size_t i = 0;
    while (i < s.size() && std::isspace((unsigned char)s[i])) ++i;
    if (i) s.erase(0, i);
    trimRight(s);
}

std::string StringUtils::demangle(const std::string& name) {
    int status = 0;
    char* res = abi::__cxa_demangle(name.c_str(), nullptr, nullptr, &status);
    std::string result = (status == 0 && res) ? res : name;
    if (res) std::free(res);
    return result;
}

std::string StringUtils::replaceExtensionWithHPP(const std::string& path) {
    size_t p = path.rfind('.');
    if (p == std::string::npos) return path + ".hpp";
    return path.substr(0, p) + ".hpp";
}

std::string StringUtils::stripNamespaceOne(const std::string& s) {
    size_t pos = s.find_last_of(':');
    if (pos != std::string::npos && pos > 0 && s[pos - 1] == ':')
        return s.substr(pos + 1);
    return s;
}

std::vector<std::string> StringUtils::splitTemplateArgs(const std::string& body) {
    std::vector<std::string> out;
    int depth = 0;
    std::string cur;
    for (size_t i = 0; i < body.size(); ++i) {
        char c = body[i];
        if (c == '<') { depth++; cur.push_back(c); }
        else if (c == '>') { depth--; cur.push_back(c); }
        else if (c == ',' && depth == 0) {
            trim(cur);
            if (!cur.empty()) out.push_back(cur);
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    trim(cur);
    if (!cur.empty()) out.push_back(cur);
    return out;
}

std::string StringUtils::sanitizeClassWithTemplates(const std::string& full) {
    size_t lt = std::string::npos, gt = std::string::npos;
    int depth = 0;
    for (size_t i = 0; i < full.size(); ++i) {
        if (full[i] == '<') {
            if (depth == 0) lt = i;
            depth++;
        }
        else if (full[i] == '>') {
            depth--;
            if (depth == 0) {
                gt = i;
                break;
            }
        }
    }

    std::string head = full;
    std::string templ;
    if (lt != std::string::npos && gt != std::string::npos && gt > lt) {
        head = full.substr(0, lt);
        templ = full.substr(lt + 1, gt - lt - 1);
    }

    auto stripNamespaceOneLocal = [](std::string s) -> std::string {
        size_t p = s.find_last_of(':');
        if (p != std::string::npos && p > 0 && s[p - 1] == ':')
            s = s.substr(p + 1);
        return s;
    };

    head = stripNamespaceOneLocal(head);
    trim(head);

    if (templ.empty()) return head;

    std::vector<std::string> args;
    std::string cur;
    int tdepth = 0;
    for (size_t i = 0; i < templ.size(); ++i) {
        char c = templ[i];
        if (c == '<') { tdepth++; cur.push_back(c); }
        else if (c == '>') { tdepth--; cur.push_back(c); }
        else if (c == ',' && tdepth == 0) {
            trim(cur);
            if (!cur.empty()) args.push_back(cur);
            cur.clear();
        }
        else cur.push_back(c);
    }
    trim(cur);
    if (!cur.empty()) args.push_back(cur);

    std::ostringstream oss;
    oss << head << "<";
    for (size_t i = 0; i < args.size(); ++i) {
        std::string a = args[i];
        trim(a);
        if (a.find('<') != std::string::npos) {
            oss << sanitizeClassWithTemplates(a);
        } else {
            bool isType = std::isalpha((unsigned char)a[0]) || a.find("::") != std::string::npos;
            if (isType) a = stripNamespaceOne(a);
            oss << a;
        }
        if (i + 1 < args.size()) oss << ", ";
    }
    oss << ">";
    return oss.str();
}

void StringUtils::stripThunkPrefixes(std::string& s) {
    const char* prefixes[] = {
            "virtual thunk to ", "non-virtual thunk to ", "covariant return thunk to ",
            "guard variable for ", "reference temporary for ", "typeinfo for ", "vtable for "
    };
    for (const char* pref : prefixes) {
        size_t p = s.find(pref);
        if (p != std::string::npos) s.erase(p, std::strlen(pref));
    }
}

std::string StringUtils::formatNamespaces(const std::string& input) {
    std::string out = input;
    static const std::vector<std::pair<std::string,std::string>> fixes = {
            {"std::__ndk1::", "std::"},
            {"std::__cxx11::", "std::"},
    };
    for (auto &f : fixes) {
        if (f.first == f.second) continue;
        size_t pos = 0;
        while ((pos = out.find(f.first, pos)) != std::string::npos) {
            out.replace(pos, f.first.size(), f.second);
            pos += f.second.size();
        }
    }
    return out;
}

void StringUtils::tightenAngles(std::string& s) {
    for (int pass = 0; pass < 3; ++pass) {
        size_t pos;
        while ((pos = s.find("> >")) != std::string::npos)
            s.replace(pos, 3, ">>");
        while ((pos = s.find(" >")) != std::string::npos && pos && s[pos - 1] == '<')
            s.erase(pos, 1);
        while ((pos = s.find("< ")) != std::string::npos)
            s.erase(pos + 1, 1);
    }
    trim(s);
}

void StringUtils::fixRemainingTemplateIssues(std::string& s) {
    size_t pos;
    while ((pos = s.find("std::vector<std::string const>")) != std::string::npos) {
        s.replace(pos, 31, "std::vector<std::string> const");
    }
    while ((pos = s.find("std::unordered_map<std::string, unsigned int><")) != std::string::npos) {
        s.replace(pos, 45, "std::unordered_map<std::string, unsigned int>");
    }
    tightenAngles(s);
}

std::string StringUtils::joinNamespace(const std::vector<std::string>& ns) {
    std::string out;
    for (size_t i = 0; i < ns.size(); ++i) {
        if (i) out += "::";
        out += ns[i];
    }
    return out;
}