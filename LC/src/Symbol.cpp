#include "Symbol.h"

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

    _grammar.push_back(rule);
}

std::vector<std::vector<std::string> > Symbol::getGramar() const
{
    return _grammar;
}
