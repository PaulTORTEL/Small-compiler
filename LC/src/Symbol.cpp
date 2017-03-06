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

void Symbol::addRule(std::vector<std::string> rule)
{

    _rules.push_back(rule);
}

std::vector<std::string> Symbol::getFirstSymbolsFromRules(const bool finalSymbol) {

    std::vector<std::string> firstSymbols;

    for (unsigned int i = 0; i < _rules.size(); i++) {

        if (_rules[i].size() > 0) { // Normalement toujours vrai

            if (finalSymbol) { // On veut des symboles terminaux
                if (!isSymbol(_rules[i][0])) // Si c'est un symbole terminal
                    firstSymbols.push_back(_rules[i][0]);
            }
            else {
                if (isSymbol(_rules[i][0])) // Si c'est un symbole non terminal
                    firstSymbols.push_back(_rules[i][0]);
            }
        }
    }
    return firstSymbols;
}

std::vector<std::vector<std::string> > Symbol::getRules() const
{
    return _rules;
}

std::vector<std::string> Symbol::getFirst() const {
    return _first;
}

std::vector<std::string> Symbol::getFollow() const {
    return _follow;
}

void Symbol::addIntoTable(std::string index, std::vector<std::string> value) {
    _table[index] = value;
}

bool Symbol::isIntoTable(std::string index) {
    return _table.find(index) != _table.end();
}

std::map<std::string, std::vector<std::string> >::iterator Symbol::findIntoTable(std::string index) {
    return _table.find(index);
}
