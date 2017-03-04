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

    std::map<std::string, Symbol*> _symbols;

    std::vector<std::string> findAndReturnFirst(const std::map<std::string, Symbol*>::iterator it); // retourne la liste de tous les symboles terminaux (utilisé dans la fonction setFirst() )

    public:
        Analyser();
        virtual ~Analyser();

        bool readGrammar(const char* filename);

        void setAllFirst(); // Fait appel à findAndReturnFirst
        void findFollow(); // Non défini


};

#endif // ANALYSER_H
