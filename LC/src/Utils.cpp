#include "Utils.h"



bool parseNumber(const char* s, int* i) {

    char* p;
    long x = strtol(s, &p, 10);
    if (*p == '\0') {
        *i = x;
        return true;
    }
    return false;
}

std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
        elems.push_back(item);

    return elems;
}


std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

bool isSymbol(std::string name) {

    for (unsigned int i = 0; i < name.size(); i++) {
        if (!(name[i] >= 65 && name[i] <= 90)) // Si ce n'est pas un symbole terminal (ce n'est pas un symbole = [A;Z]
            return false;
    }
    return true;
}

bool contains(const std::vector<std::string>& vec, const std::string s) {

    for (unsigned int i = 0; i < vec.size(); i++) {
        if (vec[i] == s)
            return true;
    }
    return false;
}
