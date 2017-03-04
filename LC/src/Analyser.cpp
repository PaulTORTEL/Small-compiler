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
            findAndSetFirst(it); // On trouve et défini les premiers d'un symbole non terminal
        }
    }
}

std::vector<std::string> Analyser::findAndSetFirst(const std::map<std::string, Symbol*>::iterator it) {

    std::vector<std::string> finalSymbols = it->second->getFirstSymbolsFromRules(true);
    std::vector<std::string> Symbols = it->second->getFirstSymbolsFromRules(false);

    if (Symbols.size() == 0) { // si les premiers sont uniquement des terminaux
        it->second->setFirst(finalSymbols); // On les sauvegarde
        return finalSymbols;    // on retourne les terminaux (, + etc)
    }


    std::vector<std::string> temp; /// va récupérer les symboles terminaux renvoyé par la récursion ex : E = T+A => renverra premier(T)
    std::vector<std::string> first; /// va stocker tous les symboles terminaux

    for (unsigned int i = 0; i < finalSymbols.size(); i++) {

        if (!contains(first, finalSymbols[i]))
            first.push_back(finalSymbols[i]);
    }

    for (unsigned int i = 0; i < Symbols.size(); i++) {

        temp = findAndSetFirst(_grammar.find(Symbols[i])); /// On ré appelle la même fonction avec les non terminaux (Premier(E) = Premier(T) donc on envoie T pour calculer ses premiers)

        for (unsigned int i = 0; i < temp.size(); i++) {
            if (!contains(first, temp[i])) { /// Si on n'a pas déjà ajouté ce symbole terminal (, + ou et etc.)
                first.push_back(temp[i]);
            }
        }
    }

    it->second->setFirst(first); // On les sauvegarde
    return first;
}

void Analyser::setAllFollow(){

    for (std::map<std::string, Symbol*>::iterator it = _grammar.begin(); it != _grammar.end(); ++it) {

        if (it->second->getFollowSize() == 0) { /// Si on n'a pas encore défini les suivants
            it->second->setFollow( findAndSetFollow(it) ); /// On trouve et défini les suivants d'un symbole non terminal
        }
    }
}

std::vector<std::string> Analyser::findAndSetFollow(const std::map<std::string, Symbol*>::iterator it) {
    std::vector<std::string> follow;

    if(_orderedSymbols.size() > 0 && it->first == _orderedSymbols[0]){
        follow.push_back("$"); /// 1ère règle: si c'est un start-symbol, on rajoute $ dans la liste des suivants
    }

    for (std::map<std::string, Symbol*>::iterator it_follow = _grammar.begin(); it_follow != _grammar.end(); ++it_follow) { ///On cherche le symbole non-terminal dans toutes les
                                                                                                                            ///règles de la grammaire
        std::vector<std::vector<std::string> > rules = it_follow->second->getRules(); ///on récupère les règles d'un symbole non terminal

        for(unsigned int i=0; i< rules.size();i++){

            for(unsigned int j=0; j< rules[i].size();j++){

                if(it->first == rules[i][j] && j == rules[i].size()-1){ ///si on a trouvé le symbole non terminal dont on cherche les suivants dans une règle dont il occupe la dernière place
                                                                        ///S’il existe une production A -> αB, les éléments de SUIVANT(A) sont ajoutés à SUIVANT(B).
                    if(it->first != it_follow->first){
                        std::vector<std::string> temp = findAndSetFollow(it_follow); ///appel de la même fonction pour trouver les suivants du symbole non terminal possédant la règle

                        for (unsigned int i = 0; i < temp.size(); i++) {
                            if (!contains(follow, temp[i])) { ///s'il n'est pas présent dans la liste sauvegardant les suivants du symbole, on le sauvegarde lui aussi
                                follow.push_back(temp[i]);
                            }
                        }
                    }
                    break; /// si on a trouvé notre symbole non terminal, il est inutile de continuer à parcourir la règle
                }

                else if(it->first == rules[i][j] && j < rules[i].size()-1){ ///si on a trouvé le symbole non terminal dont on cherche les suivants dans une règle
                                                                            ///dont il n'occupe PAS la dernière place

                    if(isSymbol(rules[i][++j])){ /// Si le symbole suivant est non terminal, alors
                        std::vector<std::string> first = _grammar[ rules[i][j] ]->getFirst();/// on récupère les premiers de  ce symbole suivant pour les sauvegarder

                        for(unsigned int k = 0; k < first.size();k++){ ///on parcourt la liste des premiers
                            if(first[k] == "#"){ ///si on trouve le mot vide
                                std::vector<std::string> temp = findAndSetFollow(_grammar.find(rules[i][j])); ///alors on cherche les suivants du symbole contenant la règle

                                for (unsigned int i = 0; i < temp.size(); i++) {
                                    if (!contains(follow, temp[i])) {
                                        follow.push_back(temp[i]);///idem que précédemment
                                    }
                                }
                            }
                            else{
                                if(!contains(follow,first[k]))
                                    follow.push_back(first[k]); ///2ème règle: S’il y a une production A-> αBβ, le contenu de PREMIER(β) excepté ε, est ajouté à SUIVANT(B).
                            }
                        }
                    }
                    else{
                        if(!contains(follow,rules[i][j]))
                            follow.push_back(rules[i][j]); ///2ème règle au cas où le caractères suivant soit un symbole terminal
                    }
                    break;///idem que précédemment
                }
            }
        }
    }
    return follow; ///on retourne la liste finale des suivants d'un symbole non terminal
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

void Analyser::displayFollow() {
    std::cout << std::endl;
    std::cout << std::endl;
    for (unsigned int i = 0; i < _orderedSymbols.size(); i++) {
        std::map<std::string, Symbol*>::iterator it = _grammar.find(_orderedSymbols[i]);
        std::cout << it->first << " a pour suivant : ";
        std::vector<std::string> vec = it->second->getFollow();
        for (unsigned int i = 0; i < it->second->getFollowSize(); i++)
            std::cout << vec[i] << " " ;

        std::cout << std::endl << std::endl;
    }

}
