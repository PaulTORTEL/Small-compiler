#ifndef SYMBOL_H
#define SYMBOL_H

#include <vector>
#include <string>

class Symbol
{
    std::string _name;

    std::vector<std::vector<std::string> > _grammar;

    std::vector<std::string> _first;
    std::vector<std::string> _follow;



    public:
        Symbol(std::string name);
        ~Symbol();

        void addRule(std::vector<std::string> rule);
        std::vector<std::vector<std::string> > getGramar() const;

};

#endif // SYMBOL_H
