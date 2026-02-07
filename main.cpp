#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>

std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

std::string replaceDots(const std::string& input) {
    std::string out;
    out.reserve(input.size());

    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '.' &&
            i > 0 && i + 1 < input.size() &&
            std::isalnum(input[i - 1]) &&
            std::isalnum(input[i + 1])) 
        {
            out += "::";
        } 
        else {
            out += input[i];
        }
    }

    return out;
}

std::vector<std::string> includes;

std::string translate(const std::string& line, bool &inMain, bool &insideClassOrNamespace) {
    std::string out = line;
    std::string t = trim(line); 
    if (!inMain && (t == "{" || t == "}")) return "";
    static std::regex usingRx(R"(using\s+([A-Za-z0-9_\.]+)\s*;)");
    std::smatch match;
    if (std::regex_search(out, match, usingRx)) {
        std::string name = match[1].str();
        std::replace(name.begin(), name.end(), '.', '/');
        includes.push_back("#include <" + name + ".h>");
        return "";
    }

    static std::regex mainRx(R"(static\s+(void|int)\s+Main\s*\()");
    if (std::regex_search(out, mainRx)) {
        inMain = true;
        return "int main() {";
    }

    if (inMain && trim(out) == "{") {
        return "";
    }

    if (inMain && out == "}") {
        inMain = false;
        return "}";
    }

    static std::regex classRx(R"(class\s+[A-Za-z0-9_]+\s*\{?)");
    if (std::regex_search(out, classRx)) {
        insideClassOrNamespace = true;
        return "";
    }

    static std::regex namespaceRx(R"(namespace\s+[A-Za-z0-9_\.]+\s*\{)");
    if (std::regex_search(out, namespaceRx)) {
        insideClassOrNamespace = true;
        return "";
    }

    if (insideClassOrNamespace && out == "}") {
        insideClassOrNamespace = false;
        return "";
    }

    out = replaceDots(out);


    return out;
}

int main(int argc, char** argv) {
    if (argc < 3) return 1;

    std::ifstream in(argv[1]);
    if (!in.is_open()) return 1;

    std::ofstream out(argv[2]);
    if (!out.is_open()) return 1;

    bool inMain = false;
    bool insideClassOrNamespace = false;

    std::string line;
    std::vector<std::string> body;

    while (std::getline(in, line)) {
        body.push_back(translate(line, inMain, insideClassOrNamespace));
    }

    while (!body.empty() && body[0].find_first_not_of(" \t\r\n") == std::string::npos) {
        body.erase(body.begin());
    }

    std::vector<std::string> cleaned;
    bool lastWasEmpty = false;

    for (auto& l : body) {
        bool isEmpty = (l.find_first_not_of(" \t\r\n") == std::string::npos);
        if (isEmpty) {
            if (!lastWasEmpty) cleaned.push_back("");
            lastWasEmpty = true;
        } else {
            cleaned.push_back(l);
            lastWasEmpty = false;
        }
    }

    body = cleaned;

    for (auto& inc : includes) out << inc << "\n";
    out << "\n";

    for (auto& l : body) out << l << "\n";

    return 0;
}
