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

    std::vector<std::string> findAndReturnFirst(const std::map<std::string, Symbol*>::iterator it); // retourne la liste de tous les symboles terminaux (utilisé dans la fonction setFirst() )

    std::vector<std::string> _orderedSymbols;


    public:
        Analyser();
        virtual ~Analyser();

        bool readGrammar(const char* filename);
        void displayGrammar();

        void setAllFirst(); // Fait appel à findAndReturnFirst
        void findFollow(); // Non défini

        void displayFirst();


};

#endif // ANALYSER_H
