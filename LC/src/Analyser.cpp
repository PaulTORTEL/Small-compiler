#include "Analyser.h"

Analyser::Analyser()
{

}

Analyser::~Analyser()
{

}

void Analyser::displayRules() {

    std::cout << std::endl << std::endl;

    std::cout << "====== [ Affichage de la grammaire ] ======" << std::endl;

    for (unsigned int i = 0; i < _orderedSymbols.size(); i++) {
        std::string symbol = _orderedSymbols[i];

        std::cout << "=== [ " << symbol << " ] ===" << std::endl;
        std::cout << "Regles : " << std::endl;

        for (unsigned int k = 0; k < _grammar[symbol]->getGramar().size(); k++) {
            std::cout << "  - ";
            for (unsigned int l = 0; l < _grammar[symbol]->getGramar()[k].size(); l++) {
                std::cout << _grammar[symbol]->getGramar()[k][l] << " ";
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }


}
