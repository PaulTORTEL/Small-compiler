#include "Analyser.h"
#include "Utils.h"
#include "Symbol.h"

bool Analyser::readGrammar(const char* filename)
{
    std::ifstream input( filename );

    std::map<std::string, Symbol*> tempMap;



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
            std::cout << "Il doit y avoir un et un seul symbole � gauche." << std::endl;
            return false;
        }

        std::cout << "Symbole : " << left[0] << std::endl ;

        std::vector<std::string> right = split(array_line[1], ' ');


        std::vector<std::vector<std::string> > rules;
        std::vector<std::string> temp_rules;

        std::cout << " - Regles : " << std::endl;
        std::cout << "  => ";
        for (unsigned int i = 1; i < right.size(); i++)
        {
            if (right[i] != "|")
            {
                temp_rules.push_back(right[i]);
                std::cout << right[i] << "  ";
            }
            else
            {
                std::vector<std::string> clone = temp_rules;
                if (clone.size() < 1)
                {
                    std::cout << "Erreur : il ne peut pas y avoir de regles vide : " << left[0] << std::endl;
                    return false;
                }
                rules.push_back(clone);
                temp_rules.clear();
                std::cout << std::endl << "  => ";
            }
        }

        if (temp_rules.size() > 0)
            rules.push_back(temp_rules);
        else
        {
            std::cout << "Erreur : il ne peut pas y avoir de regles vide : " << left[0] << std::endl;
            return false;
        }

        if (rules.size() < 1)
        {
            std::cout << "Erreur : il faut au moins une regle pour le symbole : " << left[0] << std::endl;
            return false;
        }

        std::cout << std::endl;


        if (rules[0][0] == left[0] && rules[0].size() != 1)
        {
            std::cout << "Recursivite a gauche detectee!" << std::endl;

            if (rules.size() == 1)
            {
                std::cout << "Erreur, regle non valide : " << left[0] << " = ";
                for (unsigned int i = 0; i < rules[0].size(); i++)
                {
                    std::cout << rules[0][i] << " ";
                }
                std::cout << std::endl;
                return false;
            }

            std::vector<std::string> cloned_first_rule = rules[0];

            cloned_first_rule.erase(cloned_first_rule.begin() + 0);




            std::cout << left[0] << " = ";
            for (unsigned int i = 1; i < rules.size(); i++)
            {
                std::vector<std::string> temp_symb;

                for (unsigned int j = 0; j < rules[i].size(); j++)
                {
                    temp_symb.push_back(rules[i][j]);
                }

                temp_symb.push_back(left[0] + "'");
                new_symbol_rules.push_back(temp_symb);
            }


            std::cout << left[0] << "' = ";

            std::vector<std::string> temp_new_symb_rule_left;
            std::vector<std::string> temp_new_symb_rule_right;

            for (unsigned int i = 0; i < cloned_first_rule.size(); i++)
            {
                temp_new_symb_rule_left.push_back(cloned_first_rule[i]);
            }

            temp_new_symb_rule_left.push_back(left[0] + "'");
            temp_new_symb_rule_right.push_back("#");




        }
        else
        {

            Symbol *symb = new Symbol(left[0]);
            tempMap[left[0]] = new Symbol(left[0]);

        }

        std::cout << "----------------------------------" << std::endl;

    }

    input.close();

    return true;
}
