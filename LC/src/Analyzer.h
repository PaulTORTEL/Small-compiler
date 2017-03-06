#ifndef ANALYSER_H
#define ANALYSER_H
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "Symbol.h"


class Analyzer
{

    private:


    std::string _startSymbol;


    std::map<std::string, Symbol*> _grammar;
    std::vector<std::string> _orderedSymbols;

    std::vector<std::string> findAndSetFirst(const std::map<std::string, Symbol*>::iterator it); // retourne la liste de tous les symboles terminaux (utilis� dans la fonction setFirst() )
    std::vector<std::string> findAndSetFollow(const std::map<std::string, Symbol*>::iterator it);

    std::vector<std::string> split_words(std::string sentence); // utilis� seulement par analyze(), sert � d�couper une phrase pour pouvoir l'analyser plus facilement
                                                                // (ex : vrai ou !(faux et !vrai) => vrai ou ! ( faux et ! vrai )

    public:
        Analyzer();
        virtual ~Analyzer();

        bool readGrammar(const char* filename);
        void displayGrammar();

        void setAllFirst(); // Fait appel � findAndReturnFirst
        void setAllFollow(); // Non d�fini
        void createTable(); // Cr�� la table de l'analyseur qui servira � la d�tection d'un mot

        void displayFirst();
        void displayFollow();

        void displayTable();

        bool analyze(std::string);
        void analyze_sentences();





};

#endif // ANALYSER_H
