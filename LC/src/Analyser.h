#ifndef ANALYSER_H
#define ANALYSER_H
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "Symbol.h"


class Analyser
{

    private:


    std::string _startSymbol;

    std::map<std::string, Symbol*> _grammar;
    std::vector<std::string> _orderedSymbols;

    public:
        Analyser();
        virtual ~Analyser();

        bool readGrammar(const char* filename);
        void displayRules();

};

#endif // ANALYSER_H
