#include "Analyzer.h"
#include "Utils.h"
#include <stack>
#include <algorithm>

Analyzer::Analyzer()
{

}

Analyzer::~Analyzer()
{

}

void Analyzer::displayGrammar() {

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

bool Analyzer::verifiedInfiniteLoop(std::map<std::string, Symbol*>::iterator it, std::vector<std::string>& path) {

    if (it == _grammar.end()) {
        std::cout << "Au moins 1 symbole NT n'a pas de regles (" << path.back() << ")" << std::endl;
        return false;
    }

    std::vector<std::string> temp_nextSymb = it->second->getFirstSymbolsFromRules(false);
    std::vector<std::string> nextSymbols;

    for (unsigned int i = 0; i < temp_nextSymb.size(); i++) {
        if (std::find(nextSymbols.begin(), nextSymbols.end(), temp_nextSymb[i]) == nextSymbols.end())
            nextSymbols.push_back(temp_nextSymb[i]);
    }

    path.push_back(it->first);

    if (nextSymbols.size() > 0) {
        for (unsigned int i = 0; i < nextSymbols.size(); i++) {
            if (std::find(path.begin(), path.end(), nextSymbols[i]) == path.end()) {
                path.push_back(nextSymbols[i]);
                if (!verifiedInfiniteLoop(_grammar.find(nextSymbols[i]), path))
                    return false;
            }
            else {
                std::cout << "Boucle detectee : " << nextSymbols[i] << " => ... => " << it->first << " => " << nextSymbols[i] << std::endl;
                return false;
            }
        }
    }

    return true;

}


/** TODO
        VERIFIER LES FONCTIONS FindFirstIfEpsi, createTable, getFirstByGivenRule

        On avait 2 pbs : la boucle Premier(A) = Premier(A) car B = #
        la table qui se mettait pas bien car B = #B' => ne prenait que # et ne traitait pas les transformations pour les premiers de B'

**/

bool Analyzer::setAllFirst() {

    for (std::map<std::string, Symbol*>::iterator it = _grammar.begin(); it != _grammar.end(); ++it) {

        std::vector<std::string> path; // Stocke le nom des Symboles NT qu'on a déjà croisé
        if (!verifiedInfiniteLoop(it, path))
            return false;
    }

    for (std::map<std::string, Symbol*>::iterator it = _grammar.begin(); it != _grammar.end(); ++it) {

        if (it->second->getFirstSize() == 0) { // Si on n'a pas encore défini les premiers

            std::vector<std::string> temp = findAndSetFirst(it); // On trouve et défini les premiers d'un symbole non terminal
            if (temp.size() == 1 && temp[0] == "ERROR")
                return false;
        }
    }
    return true;
}

std::vector<std::string> Analyzer::findFirstIfEpsilon(std::vector<std::string>& temp,
                                                      const int index, const unsigned int position,
                                                      const std::vector<std::vector<std::string> >& rules,
                                                      const std::string currentSymbolAnalyzed) {

    std::vector<std::string> null;

    for (unsigned int i = 0; i < temp.size(); i++) {
            //std::cout << "temp : " << temp[i] << " et le curr : " << currentSymbolAnalyzed << " index : " << index << std::endl;
            if (temp[i] == "#") {
                std::vector<std::string> temp2 ;
                //std::cout << "rules size: " << rules[index].size() << " et posi :" << position+1 << " et symb : " << rules[index][position] << " et le curr : " << currentSymbolAnalyzed << std::endl;
                if ((rules[index].size() > (position+1)) && isSymbol(rules[index][position+1])) {

                    /// SECTION A VERIFIER ABSOLUMENT
                    if (rules[index][position+1] == currentSymbolAnalyzed) { // On a trouvé A dans les règles de A (ex : A = BA, avec B = #)
                        std::cout << "Erreur : on est arrive a chercher Premier(" << currentSymbolAnalyzed << ") en cherchant Premier("
                        << rules[index][position+1] << ") car le signe precedent (" << rules[index][position] << ") donne le mot vide (#)" << std::endl;
                        std::cout << "Impossible de determiner les premiers, il est donc impossible de continuer..." << std::endl;
                        null.push_back("ERROR");
                        return null;
                    }
                    /// vérifier que le vecteur null vide est bien capable d'empêcher de calculer le reste des premiers.. il faut que le programme s'arrête absolument
                    temp2 = findAndSetFirst(_grammar.find(rules[index][position+1]));

                    std::vector<std::string> first_temp = findFirstIfEpsilon(temp2, index, position+1, rules, currentSymbolAnalyzed);

                    if (first_temp.size() == 1 && first_temp[0] == "ERROR")
                        return first_temp;

                    if (first_temp.size() > 0) {

                        if (!containsEpsilon(first_temp)) {
                            for (std::vector<std::string>::iterator it = temp2.begin(); it != temp2.end(); ++it) {
                                if (*it == "#") {
                                    temp2.erase(it);
                                    it = temp2.begin();
                                }
                            }
                        }

                        temp2.insert(temp2.end(), first_temp.begin(), first_temp.end());
                        return temp2;
                    }

                    else {
                        return temp2;
                    }
                }

                else if ((rules[index].size() > (position+1)) && !isSymbol(rules[index][position+1])) { // Si jamais on a S = A a et que A = #, on étudie alors suivant de A (a)
                    temp2.push_back(rules[index][position+1]);
                    return temp2;
                }

                else if (rules[index].size() <= position+1)
                    return temp;
            }
        }

        return null;
}

bool Analyzer::containsEpsilon(const std::vector<std::string> temp) {
    for (unsigned int j = 0; j < temp.size(); j++) {
        if (temp[j] == "#") {
            return true;
        }
    }
    return false;
}

std::vector<std::string> Analyzer::findAndSetFirst(const std::map<std::string, Symbol*>::iterator it) {

    std::vector<std::string> temp; /// va récupérer les symboles terminaux renvoyé par la récursion ex : E = T+A => renverra premier(T)

    if (it == _grammar.end()) {
        std::cout << "Un symbole est present dans une regle mais n'a aucune regle ! Impossible de continuer..." << std::endl;
        temp.push_back("ERROR");
        return temp;
    }

    std::vector<std::string> finalSymbols = it->second->getFirstSymbolsFromRules(true);
    std::vector<std::string> Symbols = it->second->getFirstSymbolsFromRules(false);
    std::vector<std::vector<std::string> > rules = it->second->getRules();

    if (Symbols.size() == 0) { // si les premiers sont uniquement des terminaux
        it->second->setFirst(finalSymbols); // On les sauvegarde

        if (!containsEpsilon(finalSymbols)) // Il n'y a pas d'epsilon
            return finalSymbols;    // on retourne les terminaux (, + etc)

        else { // Il y a au moins 1 epsilon
            for (unsigned int i = 0; i < finalSymbols.size(); i++) {
                if (finalSymbols[i] == "#" && rules[i].size() > 1 && isSymbol(rules[i][1])) { // Si epsilon est suivi d'un NT
                     Symbols.push_back(rules[i][1]);
                }
            }
        }
    }



    std::vector<std::string> first; /// va stocker tous les symboles terminaux

    for (unsigned int i = 0; i < finalSymbols.size(); i++) {

        if (!contains(first, finalSymbols[i]))
            first.push_back(finalSymbols[i]);
    }

    for (unsigned int i = 0; i < Symbols.size(); i++) {
        temp = findAndSetFirst(_grammar.find(Symbols[i])); /// On ré appelle la même fonction avec les non terminaux (Premier(E) = Premier(T) donc on envoie T pour calculer ses premiers)

        if (temp.size() == 1 && temp[0] == "ERROR")
            return temp;

        unsigned int symb_position_in_rules = 0;
        for (unsigned int rules_it = 0; rules_it < rules.size(); rules_it++) {
            if (rules[rules_it].size() > 0 && rules[rules_it][0] == Symbols[i]) { // 1ere partie normalement tjr vraie, la 2ieme teste si on a trouvé la position du symbole
                symb_position_in_rules = rules_it;
                break;
            }
        }

        std::vector<std::string> temp2 = findFirstIfEpsilon(temp, symb_position_in_rules, 0, rules, it->first);

        if (temp2.size() == 1 && temp2[0] == "ERROR")
            return temp2;

        if (!containsEpsilon(temp2)) {
            for (std::vector<std::string>::iterator it = temp.begin(); it != temp.end(); ++it) {
                if (*it == "#") {
                    temp.erase(it);
                    it--;
                }
            }
        }

        for (unsigned int j = 0; j < temp2.size(); j++) {
            if (!contains(temp, temp2[j])) { /// Si on n'a pas déjà ajouté ce symbole terminal (, + ou et etc.)
                temp.push_back(temp2[j]);
            }
        }

        for (unsigned int j = 0; j < temp.size(); j++) {

            if (!contains(first, temp[j])) { /// Si on n'a pas déjà ajouté ce symbole terminal (, + ou et etc.)
                first.push_back(temp[j]);
            }
        }
    }

    it->second->setFirst(first); // On les sauvegarde
    return first;
}

void Analyzer::setAllFollow(){

    for (std::map<std::string, Symbol*>::iterator it = _grammar.begin(); it != _grammar.end(); ++it) {

        if (it->second->getFollowSize() == 0) { /// Si on n'a pas encore défini les suivants
            it->second->setFollow( findAndSetFollow(it) ); /// On trouve et défini les suivants d'un symbole non terminal
        }
    }
}

std::vector<std::string> Analyzer::findAndSetFollow(const std::map<std::string, Symbol*>::iterator it) {
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
                }
            }
        }
    }
    return follow; ///on retourne la liste finale des suivants d'un symbole non terminal
}

void Analyzer::displayFirst() {
    std::cout << std::endl;
    std::cout << std::endl;
    for (unsigned int i = 0; i < _orderedSymbols.size(); i++) {
        std::map<std::string, Symbol*>::iterator it = _grammar.find(_orderedSymbols[i]);
        if (it != _grammar.end()) {
            std::cout << it->first << " a pour premier : ";
            std::vector<std::string> vec = it->second->getFirst();
            for (unsigned int i = 0; i < it->second->getFirstSize(); i++)
                std::cout << vec[i] << " " ;

            std::cout << std::endl << std::endl;
        }
    }

}

void Analyzer::displayFollow() {
    std::cout << std::endl;
    std::cout << std::endl;
    for (unsigned int i = 0; i < _orderedSymbols.size(); i++) {
        std::map<std::string, Symbol*>::iterator it = _grammar.find(_orderedSymbols[i]);
        if (it != _grammar.end()) {
            std::cout << it->first << " a pour suivant : ";
            std::vector<std::string> vec = it->second->getFollow();
            for (unsigned int i = 0; i < it->second->getFollowSize(); i++)
                std::cout << vec[i] << " " ;

            std::cout << std::endl << std::endl;
        }
    }
}

//Fonction A VERIFIER
std::vector<std::string> Analyzer::getFirstGivenByARule(const std::vector<std::vector<std::string> > it_rules, const int index, const int position) {

    std::map<std::string, Symbol*>::iterator temp = _grammar.find(it_rules[index][position]);
    std::vector<std::string> first_temp = temp->second->getFirst();

    if (containsEpsilon(first_temp)) {

         std::vector<std::string> temp2 = findFirstIfEpsilon(first_temp, index, position, it_rules, temp->first);

        if (temp2.size() == 1 && temp2[0] == "ERROR")
            return temp2;

         if (!containsEpsilon(temp2)) {
            for (std::vector<std::string>::iterator it_temp = first_temp.begin(); it_temp != first_temp.end(); ++it_temp) {
                if (*it_temp == "#") {
                    first_temp.erase(it_temp);
                    it_temp--;
                }
            }
        }
        for (unsigned int j = 0; j < temp2.size(); j++) {
            if (!contains(first_temp, temp2[j])) { /// Si on n'a pas déjà ajouté ce symbole terminal (, + ou et etc.)
                first_temp.push_back(temp2[j]);
            }
        }

    }

    return first_temp;
}

bool Analyzer::createTable() {

    for (std::map<std::string, Symbol*>::iterator it = _grammar.begin(); it != _grammar.end(); ++it) { // On parcourt tous les non terminaux

        std::vector<std::vector<std::string> > it_rules = it->second->getRules();
        std::vector<std::string> Symbols = it->second->getFirstSymbolsFromRules(false);

        for (unsigned int i = 0; i < it_rules.size(); i++) {

            if (it_rules[i].size() > 0) { // Normalement toujours vrai

                if (isSymbol(it_rules[i][0])) { // Symbole non terminal (ex : E => TE' ici on lit T)

                    std::vector<std::string> first_temp = getFirstGivenByARule(it_rules, i, 0); // Renvoie peut être un epsilon dans le vecteur

                    if (first_temp.size() == 1 && first_temp[0] == "ERROR")
                        return false;

                    for (std::vector<std::string>::iterator it_temp = first_temp.begin(); it_temp != first_temp.end(); ++it_temp) { // Donc on le cherche et on l'élimine
                        if (*it_temp == "#") {
                            first_temp.erase(it_temp);
                            it_temp--;
                        }
                    }

                    for (unsigned int j = 0; j < first_temp.size(); j++) {

                        if (!it->second->isIntoTable(first_temp[j])) // Si on n'a encore jamais défini de transformation pour cet index
                            it->second->addIntoTable(first_temp[j], it_rules[i]); // On ajoute dans la map du symbole non terminal la transformation
                                                                        // qui s'effectuera si on voit un symbole terminal identique à celui de l'index de la transformation
                    }
                }
                else { // C'est un symbole terminal (ex : ou et vrai faux ! etc..)  OU le mot vide # !

                    if (it_rules[i][0] == "#") {    // mot vide
                        std::vector<std::string> follow = it->second->getFollow(); // on récupère les suivants du symbole non terminal (NT) pouvant donner le mot vide
                        for (unsigned int j = 0; j < follow.size(); j++) {  // Pour chacun des symboles terminaux suivants le symbole NT
                            if (!it->second->isIntoTable(follow[j])) {
                                std::vector<std::string> temp_vec;
                                temp_vec.push_back("#");
                                it->second->addIntoTable(follow[j], temp_vec);  // On l'ajoute en tant qu'index dans la map du NT, et on définit la transformation à effectuer #
                            }
                        }

                        if (it_rules[i].size() > 1) {  // PARTIE A VERIFIER
                            if (isSymbol(it_rules[i][1])) {

                                std::vector<std::string> first_temp = getFirstGivenByARule(it_rules, i, 1);

                                if (first_temp.size() == 1 && first_temp[0] == "ERROR")
                                    return false;

                                for (std::vector<std::string>::iterator it_temp = first_temp.begin(); it_temp != first_temp.end(); ++it_temp) { // Donc on le cherche et on l'élimine
                                    if (*it_temp == "#") {
                                        first_temp.erase(it_temp);
                                        it_temp--;
                                    }
                                }

                                for (unsigned int j = 0; j < first_temp.size(); j++) {

                                    if (!it->second->isIntoTable(first_temp[j])) // Si on n'a encore jamais défini de transformation pour cet index
                                        it->second->addIntoTable(first_temp[j], it_rules[i]); // On ajoute dans la map du symbole non terminal la transformation
                                                                                    // qui s'effectuera si on voit un symbole terminal identique à celui de l'index de la transformation
                                }
                            }
                            else {
                                if (!it->second->isIntoTable(it_rules[i][1])) // Aucun transformation enregistrée à la ligne du symbole NT, colonne du symbole T
                                    it->second->addIntoTable(it_rules[i][1], it_rules[i]);  // (ex : E  et a => TE'  TE' n'est pas encore enregistré)

                                else { // Transformation déjà enregistrée ! Donc à ce stade il y a ambigüité dans la syntaxe (ex : A => a | aB) : 2 règles donnent a
                                    std::cout << "Detection d'une ambiguite dans la syntaxe de " << it->first << ". Impossible de continuer..." << std::endl;
                                    return false;
                                }
                            }
                        }

                    }
                    else {
                        if (!it->second->isIntoTable(it_rules[i][0])) // Aucun transformation enregistrée à la ligne du symbole NT, colonne du symbole T
                            it->second->addIntoTable(it_rules[i][0], it_rules[i]);  // (ex : E  et a => TE'  TE' n'est pas encore enregistré)

                        else { // Transformation déjà enregistrée ! Donc à ce stade il y a ambigüité dans la syntaxe (ex : A => a | aB) : 2 règles donnent a
                            std::cout << "Detection d'une ambiguite dans la syntaxe de " << it->first << ". Impossible de continuer..." << std::endl;
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

void Analyzer::displayTable() { // Code pas très important, on récupère les tailles max des Terminaux, Non Terminaux et on affiche le tableau en prenant ça en compte

     for (unsigned int i = 0; i < _orderedSymbols.size(); i++) {
         std::map<std::string, Symbol*>::iterator it = _grammar.find(_orderedSymbols[i]);
         if (it != _grammar.end()) {
            std::cout << it->first << " a pour table : " << std::endl;
            std::map<std::string, std::vector<std::string> >& table2 = it->second->getTable();
            for (std::map<std::string, std::vector<std::string> >::iterator it_table = table2.begin(); it_table != table2.end(); ++it_table) {
                std::cout << "\t[" << it_table->first << "] => ";
                for (unsigned int j = 0; j < it_table->second.size(); j++)
                    std::cout << it_table->second[j] << " ";
                std::cout << std::endl;
            }
            std::cout << std::endl << std::endl;
         }
     }
     std::cout << std::endl << std::endl;

    std::vector<std::string> already_counted;
    unsigned int count_max_length_rules = 0;
    unsigned int count_max_length_non_terminal = 0;

    for (std::map<std::string, Symbol*>::iterator it = _grammar.begin(); it != _grammar.end(); ++it) {
        std::vector<std::string> temp_first = it->second->getFirst();

        for (unsigned int j = 0; j < temp_first.size(); j++) {
            if (!isSymbol(temp_first[j])) {
                if (std::find(already_counted.begin(), already_counted.end(), temp_first[j]) == already_counted.end() && temp_first[j] != "#")
                    already_counted.push_back(temp_first[j]);
            }
        }

        std::vector<std::string> temp_next = it->second->getFollow();
        for (unsigned int j = 0; j < temp_next.size(); j++) {
            if (!isSymbol(temp_next[j])) {
                if (temp_next[j] != "#" && std::find(already_counted.begin(), already_counted.end(), temp_next[j]) == already_counted.end())
                    already_counted.push_back(temp_next[j]);
            }
        }

        std::vector<std::vector<std::string> > temp_rules = it->second->getRules();
        unsigned int temp_count_max = 0;

        if (count_max_length_non_terminal < it->first.length())
            count_max_length_non_terminal = it->first.length();

        for (unsigned int i = 0; i < temp_rules.size(); i++) {
            for (unsigned int j = 0; j < temp_rules[i].size(); j++) {
                if (isSymbol(temp_rules[i][j])) {
                    if (count_max_length_non_terminal < temp_rules[i][j].length())
                        count_max_length_non_terminal = temp_rules[i][j].length();
                }
                temp_count_max += temp_rules[i][j].length();
            }
            if (count_max_length_rules < temp_count_max)
                count_max_length_rules = temp_count_max;
        }
    }
    //std::cout << "ok le nb de symb T : " << already_counted.size() << " le nb de char NT : " << count_max_length_non_terminal << " le nb de char de regle : " << count_max_length_rules << std::endl;
    std::sort(already_counted.begin(), already_counted.end());

    for (unsigned int i = 0; i < count_max_length_non_terminal+1; i++)
        std::cout << " ";

    std::cout << "+";
    for (unsigned int i = 0; i < already_counted.size(); i++) {
        for (unsigned int j = 0; j < count_max_length_rules; j++)
            std::cout << "-";

        std::cout << "+";

    }
    std::cout << std::endl;

    for (unsigned int i = 0; i < count_max_length_non_terminal; i++)
        std::cout << " ";
    std::cout << " |";
    for (unsigned int i = 0; i < already_counted.size(); i++) {
        std::cout << already_counted[i];
        for (unsigned int j = 0; j < (count_max_length_rules - already_counted[i].length()); j++)
            std::cout << " ";
        std::cout << "|";
    }
    std::cout << std::endl;

    for (std::map<std::string, Symbol*>::iterator it = _grammar.begin(); it != _grammar.end(); ++it) {
        std::cout << "+";
        for (unsigned int i = 0; i < count_max_length_non_terminal; i++)
                std::cout << "-";
        std::cout << "+";


        for (unsigned int i = 0; i < already_counted.size(); i++) {
            for (unsigned int j = 0; j < count_max_length_rules; j++)
                std::cout << "-";

            std::cout << "+";
        }
        std::cout << std::endl << "|";

        std::cout << it->first;

        for (unsigned int i = 0; i < (count_max_length_non_terminal - it->first.length()); i++)
            std::cout << " ";
        std::cout << "|";

        std::map<std::string, std::vector<std::string> >& table = it->second->getTable();

        for (unsigned int i = 0; i < already_counted.size(); i++) {
            if (table.find(already_counted[i]) == table.end()) {
                for (unsigned int j = 0; j < count_max_length_rules; j++)
                    std::cout << " ";
                std::cout << "|";
            }
            else {
                unsigned int temp_size = 0;
                std::vector<std::string> temp_vec = table[already_counted[i]];
                for (unsigned int j = 0; j < temp_vec.size(); j++) {
                    std::cout << temp_vec[j];
                    temp_size += temp_vec[j].length() ;
                }
                for (unsigned int j = 0; j < (count_max_length_rules - temp_size); j++)
                    std::cout << " ";
                std::cout << "|";
            }
        }

        std::cout << std::endl;
    }

    std::cout << "+";
    for (unsigned int i = 0; i < count_max_length_non_terminal; i++)
            std::cout << "-";
    std::cout << "+";


    for (unsigned int i = 0; i < already_counted.size(); i++) {
        for (unsigned int j = 0; j < count_max_length_rules; j++)
            std::cout << "-";

        std::cout << "+";

    }
    std::cout << std::endl;
}

std::vector<std::string> Analyzer::split_words(std::string sentence){

    std::vector<std::string> returned_vector;
    unsigned int i = 0;
    std::string temp;

    while(i < sentence.length()){ //On parcourt la chaîne de caractère

        if((sentence[i] > 65 && sentence[i] < 90)||(sentence[i] > 96 && sentence[i] < 122)){ //si c'est une lettre, on le met dans la string temporaire
            temp += sentence[i];
        }
        else if (sentence[i] == ' '){ //si c'est un espace, alors le mot est fini et on le stocke dans le vecteur
            if(!temp.empty())
                returned_vector.push_back(temp);
            temp.clear();
        }
        else{ //si c'est un caractère spécial comme: ( , ) , ! , : ...
            if(!temp.empty()){ //si le vecteur n'est pas vide, on enregiste le mot avant de le supprimer de la chaîne de caractère
                returned_vector.push_back(temp);
                temp.clear();
            }
            temp += sentence[i];
            returned_vector.push_back(temp); //puis on stocke le caractère spécial dans le vecteur
            temp.clear();
        }
        i++;
    }
    return returned_vector; //on retourne le vecteur
}


bool Analyzer::analyze(std::string sentence) {

    sentence += " $"; // On met à la fin du mot $ pour indiquer la fin de la phrase
    std::vector<std::string> splited_sentence = split_words(sentence);

    std::stack<std::string> stack;
    stack.push("$");

    if (_orderedSymbols.size() > 0) {
        stack.push(_orderedSymbols[0]); // Start symbole mis
    }

    while (stack.size() > 0) {
        if (stack.top() == "$" && splited_sentence.size() > 1) // Si jamais on atteint $ sur la pile et qu'il y a encore des termes à reconnaître dans la phrase (autre que $)
            return false;                                       // S'il ne reste que $ à reconnaître dans la phrase alors la taille == 1

        if (stack.top() == "#")
            stack.pop();

        else {
            if (isSymbol(stack.top())) {
                std::map<std::string, Symbol*>::iterator it = _grammar.find(stack.top());
                if (it != _grammar.end()) {
                    if (splited_sentence.size() > 0) {
                        std::map<std::string, std::vector<std::string> >::iterator it_table = it->second->findIntoTable(splited_sentence[0]);

                        if (it_table == it->second->getTable().end()) // Mot non reconnu
                            return false;

                        else {
                            stack.pop();
                            std::vector<std::string> transformation = it_table->second;

                            for (int i = transformation.size()-1; i >= 0; i--) {
                                stack.push(transformation[i]);
                            }
                        }
                    }
                }
                else
                    return false;
            }
            else { // C'est un terminal
                if (stack.top() == splited_sentence[0]) { // Portion reconnue
                    stack.pop();                                         // On enlève de la pile le terminal
                    splited_sentence.erase(splited_sentence.begin());   // On enlève de la phrase à reconnaître le terminal
                }
                else
                    return false;
            }
        }
    }
    return true; // Mot reconnu
}

void Analyzer::analyze_sentences() {

    while (true) {
        std::cout << "/stop pour arreter la reconnaissance de mots" << std::endl;
        std::cout << "Quel mot a reconnaitre ?" << std::endl;
            std::string sentence;
            getline(std::cin, sentence);

            if (sentence == "/stop")
                break;

            if (analyze(sentence))
                std::cout << "\tMot reconnu !" << std::endl;

            else
                std::cout << "\tMot non reconnu..." << std::endl;

            std::cout << std::endl;
    }
}
