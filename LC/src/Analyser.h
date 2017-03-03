#ifndef ANALYSER_H
#define ANALYSER_H
#include <iostream>
#include <fstream>
#include <string>
#include <map>


class Analyser
{

    private:

    /*std::map<std::string, std::vector<std::string> > _grammar;*/
    std::string _startSymbol;

    public:
        Analyser();
        virtual ~Analyser();

        bool readGrammar(const char* filename);

};

#endif // ANALYSER_H
