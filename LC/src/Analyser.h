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

<<<<<<< HEAD
    /*std::map<std::string, std::vector<std::string> > _grammar;*/
    std::string _startSymbol;
=======
    std::map<std::string, Symbol*> _grammar;
>>>>>>> 9d5c61046abe3a6b0db36618536a9403cb2a84ac

    public:
        Analyser();
        virtual ~Analyser();

        bool readGrammar(const char* filename);

};

#endif // ANALYSER_H
