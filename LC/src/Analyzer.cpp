#include "Analyzer.h"
#include "Utils.h"
#include <stack>

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


void Analyzer::setAllFirst() {

    for (std::map<std::string, Symbol*>::iterator it = _grammar.begin(); it != _grammar.end(); ++it) {

        if (it->second->getFirstSize() == 0) { // Si on n'a pas encore défini les premiers
            findAndSetFirst(it); // On trouve et défini les premiers d'un symbole non terminal
        }
    }
}

std::vector<std::string> Analyzer::findAndSetFirst(const std::map<std::string, Symbol*>::iterator it) {

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

void Analyzer::createTable() {

    for (std::map<std::string, Symbol*>::iterator it = _grammar.begin(); it != _grammar.end(); ++it) { // On parcourt tous les non terminaux
                                                                                                                                            //               ou
        std::vector<std::vector<std::string> > it_rules = it->second->getRules();

        for (unsigned int i = 0; i < it_rules.size(); i++) {
            if (it_rules[i].size() > 0) { // Normalement toujours vrai

                if (isSymbol(it_rules[i][0])) { // Symbole non terminal (ex : E => TE' ici on lit T)
                    std::map<std::string, Symbol*>::iterator temp = _grammar.find(it_rules[i][0]);
                    std::vector<std::string> first_temp = temp->second->getFirst();

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
                    }
                    else {
                        if (!it->second->isIntoTable(it_rules[i][0]))
                            it->second->addIntoTable(it_rules[i][0], it_rules[i]);
                    }
                }
            }
        }
    }
}

void Analyzer::displayTable() {

     for (unsigned int i = 0; i < _orderedSymbols.size(); i++) {
         std::map<std::string, Symbol*>::iterator it = _grammar.find(_orderedSymbols[i]);
         if (it != _grammar.end()) {
            std::cout << it->first << " a pour table : " << std::endl;
            for (std::map<std::string, std::vector<std::string> >::iterator it_table = it->second->getTableBegin(); it_table != it->second->getTableEnd(); ++it_table) {
                std::cout << "\t[" << it_table->first << "] => ";
                for (unsigned int j = 0; j < it_table->second.size(); j++)
                    std::cout << it_table->second[j] << " ";
                std::cout << std::endl;
            }
            std::cout << std::endl << std::endl;
         }
     }
}

std::vector<std::string> Analyzer::split_words(std::string sentence){

    std::vector<std::string> returned_vector;
    unsigned int i=0;
    std::string temp;

    while(i < sentence.length()){ //On parcoure la chaîne de caractère

        if((sentence[i] < 90 && sentence[i] > 65)||(sentence[i] > 96 && sentence[i] < 122)){ //si c'est une lettre, on le met dans la string temporaire
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
    ///std::vector<std::string> splited_sentence = split(sentence, ' ');
    std::vector<std::string> splited_sentence = split_words(sentence);
    std::cout << "phrase : ";
    for(unsigned int i=0; i < splited_sentence.size();i++){
        std::cout << splited_sentence[i] << " ";
    }

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

                        if (it_table == it->second->getTableEnd()) // Mot non reconnu
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
            }
        }
    }
    return true; // Mot reconnu
}

void Analyzer::analyze_sentences() {

    while (true) {
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