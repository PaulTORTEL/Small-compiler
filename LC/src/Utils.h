#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <cstdlib>
#include <sstream>
#include <vector>

bool parseNumber(const char* s, int* i);
std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
bool isSymbol(std::string name); // Renvoie true si le symbole est Non Terminal ([A;Z])
bool contains(const std::vector<std::string>& vec, const std::string s); // Renvoie true si le string est contenu dans une case du vector
#endif // UTILS_H_INCLUDED
