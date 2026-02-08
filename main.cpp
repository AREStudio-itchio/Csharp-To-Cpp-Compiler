#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <cctype>

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

std::string translate(const std::string& line, bool &inMain, bool &insideClass) {
    std::string out = line;
    std::string t = trim(line);

    // Eliminar llaves sueltas de clases C#
    if (!inMain && (t == "{" || t == "}")) return "";

    // using → include
    static std::regex usingRx(R"(using\s+([A-Za-z0-9_\.]+)\s*;)");
    std::smatch match;
    if (std::regex_search(t, match, usingRx)) {
        std::string name = match[1].str();
        std::replace(name.begin(), name.end(), '.', '/');
        includes.push_back("#include <" + name + ".h>");
        return "";
    }

    // Detectar Main
    static std::regex mainRx(R"(static\s+(void|int)\s+Main\s*\()");
    if (std::regex_search(t, mainRx)) {
        inMain = true;
        return "int main() {";
    }

    // Ignorar llaves dentro del Main
    if (inMain && t == "{") return "";
    if (inMain && t == "}") {
        inMain = false;
        return "}";
    }

    // Detectar class Game { ... }
    static std::regex classRx(R"(class\s+[A-Za-z0-9_]+\s*\{?)");
    if (std::regex_search(t, classRx)) {
        insideClass = true;
        return "";
    }

    // Cerrar class
    if (insideClass && t == "}") {
        insideClass = false;
        return "";
    }

    // Reemplazar dots por ::
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
    bool insideClass = false;

    std::string line;
    std::vector<std::string> body;

    while (std::getline(in, line)) {
        body.push_back(translate(line, inMain, insideClass));
    }

    // Limpiar líneas vacías al inicio
    while (!body.empty() && trim(body[0]).empty()) {
        body.erase(body.begin());
    }

    // Compactar líneas vacías
    std::vector<std::string> cleaned;
    bool lastEmpty = false;

    for (auto &l : body) {
        bool empty = trim(l).empty();
        if (empty) {
            if (!lastEmpty) cleaned.push_back("");
            lastEmpty = true;
        } else {
            cleaned.push_back(l);
            lastEmpty = false;
        }
    }

    body = cleaned;

    // Escribir includes
    for (auto &inc : includes) out << inc << "\n";
    out << "\n";

    // Escribir cuerpo traducido
    for (auto &l : body) out << l << "\n";

    return 0;
}
