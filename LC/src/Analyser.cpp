#include "Analyser.h"
#include "Utils.h"

Analyser::Analyser()
{

}

Analyser::~Analyser()
{

}

void Analyser::displayGrammar() {

    std::cout << std::endl << std::endl;

    std::cout << "====== [ Affichage de la grammaire ] ======" << std::endl;

    for (unsigned int i = 0; i < _orderedSymbols.size(); i++) {
        std::string symbol = _orderedSymbols[i];

        std::cout << "=== [ " << symbol << " ] ===" << std::endl;
        std::cout << "Regles : " << std::endl;

        for (unsigned int k = 0; k < _grammar[symbol]->getRules().size(); k++) {
            std::cout << "  - ";
            for (unsigned int l = 0; l < _grammar[symbol]->getRules()[k].size(); l++) {
                std::cout << _grammar[symbol]->getRules()[k][l] << " ";
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }


}


void Analyser::setAllFirst() {

    for (std::map<std::string, Symbol*>::iterator it = _grammar.begin(); it != _grammar.end(); ++it) {

        if (it->second->getFirstSize() == 0) { // Si on n'a pas encore défini les premiers

            std::vector<std::string> first = findAndReturnFirst(it); // On trouve les premiers d'un symbole non terminal
            it->second->setFirst(first); // On les sauvegarde
        }
    }
}

std::vector<std::string> Analyser::findAndReturnFirst(const std::map<std::string, Symbol*>::iterator it) {

    std::vector<std::string> finalSymbols = it->second->getFirstSymbolsFromRules(true);
    std::vector<std::string> Symbols = it->second->getFirstSymbolsFromRules(false);

    if (Symbols.size() == 0)  // si les premiers sont uniquement des terminaux
        return finalSymbols;    // on retourne les terminaux (, + etc)

    std::vector<std::string> temp; // va récupérer les symboles terminaux renvoyé par la récursion ex : E = T+A => renverra premier(T)
    std::vector<std::string> first; // va stocker tous les symboles terminaux

    for (unsigned int i = 0; i < finalSymbols.size(); i++) {

        if (!contains(first, finalSymbols[i]))
            first.push_back(finalSymbols[i]);
    }

    for (unsigned int i = 0; i < Symbols.size(); i++) {
        temp = findAndReturnFirst(_grammar.find(Symbols[i])); // On ré appelle la même fonction avec les non terminaux (Premier(E) = Premier(T) donc on envoie T pour calculer ses premiers)
        for (unsigned int i = 0; i < temp.size(); i++) {
            if (!contains(first, temp[i])) { // Si on n'a pas déjà ajouté ce symbole terminal (, + ou et etc.)
                first.push_back(temp[i]);
            }
        }
    }

    return first;
}

void Analyser::findFollow() {

}

void Analyser::displayFirst() {
    std::cout << std::endl;
    std::cout << std::endl;
    for (unsigned int i = 0; i < _orderedSymbols.size(); i++) {
        std::map<std::string, Symbol*>::iterator it = _grammar.find(_orderedSymbols[i]);
        std::cout << it->first << " a pour premier : ";
        std::vector<std::string> vec = it->second->getFirst();
        for (unsigned int i = 0; i < it->second->getFirstSize(); i++)
            std::cout << vec[i] << " " ;

        std::cout << std::endl << std::endl;
    }

}
