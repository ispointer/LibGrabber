
/*
 * Created by aantik
 * 3/27/2026 4:30 PM
 *
 *   ⋆    ႔ ႔
 *     ᠸ^ ^ ⸝⸝
 *       |、˜〵
 *      じしˍ,)⁐̤ᐷ
 *
 * Fox Mode 🍺
 */


#include "HeaderWriter.h"
#include "../utils/StringUtils.h"
#include "../utils/TypeUtils.h"
#include <iomanip>
#include <functional>
#include <algorithm>
#include <regex>
#include <map>
#include <sstream>

static std::string extractAccess(const std::string& sig) {
    if (sig.find("public") != std::string::npos) return "public";
    if (sig.find("private") != std::string::npos) return "private";
    if (sig.find("protected") != std::string::npos) return "protected";
    if (sig.find("internal") != std::string::npos) return "public";
    return "public";
}

static void cleanMethodName(std::string& sig) {
    sig = std::regex_replace(sig, std::regex(R"(.*\.)"), "");
}

void HeaderWriter::write(std::ostream& out,
                         const std::map<std::vector<std::string>, std::set<FunctionInfo>>& freeFunctions,
                         const std::map<ClassKey, std::set<MethodInfo>>& classes) {

    std::set<std::vector<std::string>> allNamespaces;
    for (const auto& kv : classes) allNamespaces.insert(kv.first.first);
    for (const auto& kv : freeFunctions) allNamespaces.insert(kv.first);

    std::vector<std::vector<std::string>> sortedNamespaces(allNamespaces.begin(), allNamespaces.end());
    std::sort(sortedNamespaces.begin(), sortedNamespaces.end());

    auto indent = [](int n) { return std::string(n * 4, ' '); };

    struct NamespaceNode {
        std::string name;
        std::map<std::string, NamespaceNode> children;
        std::set<FunctionInfo> functions;
        std::map<std::string, std::set<MethodInfo>> classes;
    };

    NamespaceNode root;

    for (const auto& nsPath : sortedNamespaces) {
        NamespaceNode* current = &root;
        for (const auto& nsPart : nsPath) {
            current = &current->children[nsPart];
        }

        auto ffIt = freeFunctions.find(nsPath);
        if (ffIt != freeFunctions.end()) {
            current->functions = ffIt->second;
        }

        for (const auto& kv : classes) {
            if (kv.first.first == nsPath) {
                current->classes[kv.first.second] = kv.second;
            }
        }
    }

    auto formatMethodSignature = [&](std::string sig, std::string ret)
            -> std::pair<std::string, std::string> {

        sig = TypeUtils::simplifyTypes(sig);
        ret = TypeUtils::simplifyTypes(ret);

        cleanMethodName(sig);

        std::regex r(R"((\w+)\s*\(([^)]*)\))");
        std::smatch m;

        if (!std::regex_search(sig, m, r)) {
            return {sig, ret};
        }

        std::string name = m[1];
        std::string params = m[2];

        std::vector<std::string> paramList;
        std::string cur;
        int depth = 0;

        for (char c : params) {
            if (c == '<') depth++;
            if (c == '>') depth--;
            if (c == ',' && depth == 0) {
                StringUtils::trim(cur);
                if (!cur.empty()) paramList.push_back(cur);
                cur.clear();
            } else {
                cur += c;
            }
        }

        if (!cur.empty()) {
            StringUtils::trim(cur);
            if (!cur.empty()) paramList.push_back(cur);
        }

        if (paramList.size() == 1) {
            if (ret.empty() || ret == "void") {
                ret = paramList[0];
            }
            return {name + "()", ret};
        }

        if (paramList.size() > 1) {
            std::stringstream ss;
            for (size_t i = 0; i < paramList.size(); ++i) {
                if (i > 0) ss << ", ";
                ss << paramList[i] << " arg" << i;
            }
            return {name + "(" + ss.str() + ")", "void"};
        }

        return {name + "()", ret};
    };

    std::function<void(const NamespaceNode&, int)> outputNode;

    outputNode = [&](const NamespaceNode& node, int depth) {

        if (!node.name.empty()) {
            out << indent(depth) << "namespace " << node.name << " {\n\n";
            depth++;
        }

        for (const auto& cls : node.classes) {
            std::string className = cls.first.empty() ? "Unknown" : TypeUtils::simplifyTypes(cls.first);

            out << indent(depth) << "class " << className << " {\n";
            out << indent(depth + 1) << "public:\n";

            for (const auto& m : cls.second) {

                out << indent(depth + 1) << "    // Offset: 0x"
                    << std::hex << m.address
                    << " RVA: 0x" << m.address
                    << std::dec << "\n";

                std::string access = extractAccess(m.signature);

                auto formatted = formatMethodSignature(m.signature, m.returnType);

                if (!formatted.second.empty() && formatted.second != "void") {
                    out << indent(depth + 1)
                        << "    " << access << " "
                        << formatted.second << " "
                        << formatted.first << " { }\n";
                } else {
                    out << indent(depth + 1)
                        << "    " << access << " void "
                        << formatted.first << " { }\n";
                }
            }

            out << indent(depth) << "};\n\n";
        }

        for (const auto& child : node.children) {
            outputNode(child.second, depth);
        }

        if (!node.name.empty()) {
            depth--;
            out << indent(depth) << "} // namespace " << node.name << "\n\n";
        }
    };

    outputNode(root, 0);
}