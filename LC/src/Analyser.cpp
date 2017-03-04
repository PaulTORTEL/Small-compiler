#include "Analyser.h"
#include "Utils.h"

Analyser::Analyser()
{
    //ctor
}

Analyser::~Analyser()
{
    //dtor
}


void Analyser::setAllFirst() {

    for (std::map<std::string, Symbol*>::iterator it = _symbols.begin(); it != _symbols.end(); ++it) {

        if (it->second->getFirstSize() == 0) { // Si on n'a pas encore défini les premiers

            std::vector<std::string> first = findAndReturnFirst(it);
            it->second->setFirst(first);
        }
    }
}

std::vector<std::string> Analyser::findAndReturnFirst(const std::map<std::string, Symbol*>::iterator it) {

    std::vector<std::string> finalSymbols = it->second->getFirstSymbolsFromGrammar(true);
    std::vector<std::string> Symbols = it->second->getFirstSymbolsFromGrammar(false);

    if (Symbols.size() == 0)    // si les premiers sont uniquement des terminaux
        return finalSymbols;    // on retourne les terminaux (, + etc)

    std::vector<std::string> temp; // va récupérer les symboles terminaux renvoyé par la récursion ex : E = T+A => renverra premier(T)
    std::vector<std::string> first; // va stocker tous les symboles terminaux

    for (unsigned int i = 0; i < finalSymbols.size(); i++) {
        if (!contains(first, finalSymbols[i]))
            first.push_back(finalSymbols[i]);
    }

    for (unsigned int i = 0; i < Symbols.size(); i++) {

        temp = findAndReturnFirst(_symbols.find(Symbols[i]));

        for (unsigned int i = 0; i < temp.size(); i++) {
            if (!contains(first, temp[i]))
                first.push_back(finalSymbols[i]);
        }
    }

    return first;
}

void Analyser::findFollow() {

}
