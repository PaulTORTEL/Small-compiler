#include <iostream>
#include "Analyzer.h"
#include <dirent.h>
#include <stdlib.h>

std::vector<std::string> GetGrammars(const char *path) {
    std::vector<std::string> files;
    struct dirent *entry;
    DIR *dp;

    dp = opendir(path);
    if (dp == NULL) {
        return files;
    }

    while ((entry = readdir(dp))) {
        std::string filename(entry->d_name);

        if (filename.find(".txt") != std::string::npos) {
            files.push_back(filename);
        }
    }
    closedir(dp);
    return files;
}

const std::string* ChooseGrammar(const std::vector<std::string>& grammars) {
    std::cout << "Choisir un fichier a charger :" << std::endl;
    std::cout << "0. Quitter" << std::endl;
    for (unsigned int i = 0; i < grammars.size(); i++) {
        std::cout << (i + 1) << ". " << grammars[i] << std::endl;
    }

    unsigned int choice = 0;
    while (choice <= 0 || choice > grammars.size()) {
        std::cout << "Choix : ";
        std::cin >> choice;
        if(std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');

        } else {
            std::cin.ignore(1, '\n');
            if(choice == 0)
                return NULL;
        }
    }

    return &grammars[choice-1];
}

int main()
{
    const char* folder = "grammars/";
    std::vector<std::string> grammars = GetGrammars(folder);

    if(grammars.empty()) {
        std::cout << std::endl << "\t*=======*=======* Analyseur descendant *=======*=======*" << std::endl << std::endl;
        std::cout << "\tDevs: Vincent COURSAC, Paul TORTEL & Matthieu PILAUDEAU " << std::endl << std::endl;
        std::cerr << "Le dossier " << folder << " ne contient aucune grammaire ou est inexistant!" << std::endl;
        return(1);
    }

    while (true) {
        std::cout << std::endl << "\t*=======*=======* Analyseur descendant *=======*=======*" << std::endl << std::endl;
        std::cout << "\tDevs: Vincent COURSAC, Paul TORTEL & Matthieu PILAUDEAU " << std::endl << std::endl;
        const std::string* choice = ChooseGrammar(grammars);

        if(!choice)
            break;

        Analyzer analyzer;
        std::string temp = (std::string)folder + "/" + *choice;
        if (analyzer.readGrammar(temp.c_str())) {
            analyzer.displayGrammar();
            if (analyzer.setAllFirst()) {
                analyzer.displayFirst();
                analyzer.setAllFollow();
                analyzer.displayFollow();
                if (analyzer.createTable()) {
                    analyzer.displayTable();
                    analyzer.analyze_sentences();
                }
            }

        }
        else {
            std::cerr << "Impossible de lire le fichier..." << std::endl;
        }
        system("PAUSE");
        system("CLS");
    }
    return 0;
}
