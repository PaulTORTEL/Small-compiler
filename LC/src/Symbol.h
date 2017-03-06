#ifndef SYMBOL_H
#define SYMBOL_H

#include <vector>
#include <string>
#include <map>

class Symbol
{
    std::string _name;

    std::vector<std::vector<std::string> > _rules;

    std::vector<std::string> _first;
    std::vector<std::string> _follow;

    std::map<std::string, std::vector<std::string> > _table;    // Table pour le symbole Non terminal        vrai    faux  ...
                                                                //                                      E     TE'     TE'   ...
                                                                //  index : vrai, faux etc. vector de string : [T][E']


    public:
        Symbol(std::string name);
        ~Symbol();

        void addRule(std::vector<std::string> rule);

        std::vector<std::string> getFirstSymbolsFromRules(const bool finalSymbol); // Récupère les symboles terminaux (si finalSymbol = true) ou les symboles non terminaux ( = false) à partir de la grammaire

        void setFirst(std::vector<std::string> first);
        void setFollow(std::vector<std::string> follow); // non défini pour le moment

        unsigned int getFirstSize();
        unsigned int getFollowSize();

        std::vector<std::vector<std::string> > getRules() const;
        std::vector<std::string> getFirst() const;
        std::vector<std::string> getFollow() const;

        void addIntoTable(std::string index, std::vector<std::string> value);
        bool isIntoTable(std::string index);
        std::map<std::string, std::vector<std::string> >::iterator findIntoTable(std::string index);

        std::map<std::string, std::vector<std::string> >::iterator getTableBegin();
        std::map<std::string, std::vector<std::string> >::iterator getTableEnd();

};

inline void Symbol::setFirst(std::vector<std::string> first) {
    _first = first;
}

inline void Symbol::setFollow(std::vector<std::string> follow) {
    _follow = follow;
}

inline unsigned int Symbol::getFirstSize() {
    return _first.size();
}

inline unsigned int Symbol::getFollowSize() {
    return _follow.size();
}

inline std::map<std::string, std::vector<std::string> >::iterator Symbol::getTableBegin() {
    return _table.begin();
}

inline std::map<std::string, std::vector<std::string> >::iterator Symbol::getTableEnd() {
    return _table.end();
}
#endif // SYMBOL_H
