
/*
 * Created by aantik 
 * 3/27/2026 4:26 PM
 *
 *   ⋆    ႔ ႔
 *     ᠸ^ ^ ⸝⸝
 *       |、˜〵
 *      じしˍ,)⁐̤ᐷ
 *
 * Fox Mode 🍺
 */


#ifndef GRABBER_MODELS_SYMBOLINFO_H
#define GRABBER_MODELS_SYMBOLINFO_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include <elfio/elfio.hpp>

using namespace ELFIO;

struct FunctionInfo {
    std::string returnType;
    std::string signature;
    Elf64_Addr address;

    FunctionInfo() : address(0) {}
    FunctionInfo(const std::string& ret, const std::string& sig, Elf64_Addr addr)
            : returnType(ret), signature(sig), address(addr) {}

    bool operator<(const FunctionInfo& other) const {
        if (signature != other.signature) return signature < other.signature;
        if (returnType != other.returnType) return returnType < other.returnType;
        return address < other.address;
    }
};

struct MethodInfo {
    std::string returnType;
    std::string signature;
    Elf64_Addr address;
    bool isConstructor = false;
    bool isDestructor = false;

    MethodInfo() : address(0) {}
    MethodInfo(const std::string& ret, const std::string& sig, Elf64_Addr addr)
            : returnType(ret), signature(sig), address(addr) {}

    bool operator<(const MethodInfo& other) const {
        if (signature != other.signature) return signature < other.signature;
        if (returnType != other.returnType) return returnType < other.returnType;
        return address < other.address;
    }
};

using ClassKey = std::pair<std::vector<std::string>, std::string>;

#endif