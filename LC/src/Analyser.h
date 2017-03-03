#ifndef ANALYSER_H
#define ANALYSER_H
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "Symbol.h"

class Analyser
{
    std::string _startSymbol;

    private:

    std::map<std::string, Symbol*> _grammar;

    public:
        Analyser();
        virtual ~Analyser();

        bool readGrammar(const char* filename);

};

#endif // ANALYSER_H
