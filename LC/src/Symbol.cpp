#include "Symbol.h"
#include "Utils.h"

Symbol::Symbol(std::string name)
{
    _name = name;
}

Symbol::~Symbol()
{
    //dtor
}

void Symbol::addRule(std::vector<std::string> rule) {

    _grammar.push_back(rule);
}

std::vector<std::string> Symbol::getFirstSymbolsFromGrammar(const bool finalSymbol) {

    std::vector<std::string> firstSymbols;

    for (unsigned int i = 0; i < _grammar.size(); i++) {

        if (_grammar[i].size() > 0) { // Normalement toujours vrai

            if (finalSymbol) { // On veut des symboles terminaux
                if (!isSymbol(_grammar[i][0])) // Si c'est un symbole terminal
                    firstSymbols.push_back(_grammar[i][0]);
            }
            else {
                if (isSymbol(_grammar[i][0])) // Si c'est un symbole non terminal
                    firstSymbols.push_back(_grammar[i][0]);
            }
        }
    }
    return firstSymbols;
}
