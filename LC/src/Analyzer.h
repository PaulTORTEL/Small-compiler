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

    std::vector<std::string> findAndSetFirst(const std::map<std::string, Symbol*>::iterator it); // retourne la liste de tous les symboles terminaux (utilisé dans la fonction setFirst() )
    std::vector<std::string> findAndSetFollow(const std::map<std::string, Symbol*>::iterator it);

    public:
        Analyzer();
        virtual ~Analyzer();

        bool readGrammar(const char* filename);
        void displayGrammar();

        void setAllFirst(); // Fait appel à findAndReturnFirst
        void setAllFollow(); // Non défini
        void createTable(); // Créé la table de l'analyseur qui servira à la détection d'un mot

        void displayFirst();
        void displayFollow();

        void displayTable();

        bool analyze(std::string);
        void analyze_sentences();



};

#endif // ANALYSER_H
