#include "Analyser.h"
#include "Utils.h"

bool Analyser::readGrammar(const char* filename)
{
    std::ifstream input( filename );

    std::vector<std::string> unknowned_symbols;



    for( std::string line; getline( input, line ); )
    {
        std::vector<std::string> array_line = split(line, '=');
        if (array_line.size() != 2)
        {
            std::cout << "Erreur : " << line << std::endl;
            std::cout << "Vous devez utilisez la syntaxe suivante : [Symbole] = [Grammaire du symbole]" << std::endl;
            return false;
        }

        std::vector<std::string> left = split (array_line[0], ' ');
        if (left.size() != 1)
        {
            std::cout << "Erreur : " << line << std::endl;
            std::cout << "[Symbole] = [Grammaire du symbole]" << std::endl;
            std::cout << "Il doit y avoir un et un seul symbole à gauche." << std::endl;
            return false;
        }

        std::cout << "Symbole : " << left[0] << std::endl;

        std::vector<std::string> right = split(array_line[1], ' ');

        for (unsigned int i = 0; i < right.size(); i++) {

        }

    }

    input.close();

    return true;
}
