#include "Analyzer.h"
#include "Utils.h"
#include "Symbol.h"

bool Analyzer::readGrammar(const char* filename)
{

    std::ifstream input( filename );

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

        std::vector<std::string> right = split(array_line[1], ' ');


        std::vector<std::vector<std::string> > rules;
        std::vector<std::string> temp_rules;


        for (unsigned int i = 1; i < right.size(); i++)
        {
            if (right[i] != "|")
            {
                temp_rules.push_back(right[i]);
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


        for (unsigned int i1 = 0; i1 < rules.size(); i1++)
        {
            for (unsigned int i2 = 0; i2 < rules[i1].size(); i2++)
            {
                if (rules[i1][i2].size() == 0)
                {
                    std::cout << "Erreur : \n" << line << std::endl;
                    for (unsigned int f = 0; f < line.size() ; f++)
                    {
                        if (line[f] == ' ')
                        {
                            std::cout << "E";
                        }
                        else
                        {
                            std::cout << "X";
                        }
                    }
                    std::cout << std::endl;
                    std::cout << "[Symbole] = [Regle] [Regle] ... [Regle]" << std::endl;
                    std::cout << "[Regle] a pour format : [Symb] [Symb] [Symb]" << std::endl;
                    std::cout << "Syntaxe incorrecte." << std::endl;
                    return false;
                }

                for (unsigned int i3 = 0; i3 < rules[i1][i2].size(); i3++)
                {
                    if (rules[i1][i2][i3] == ' ' || rules[i1][i2][i3] == '|' || rules[i1][i2][i3] == '\t' )
                    {
                        std::cout << "Erreur : \n" << line << std::endl;
                        for (unsigned int f = 0; f < line.size() ; f++)
                        {
                            if (line[f] == ' ')
                            {
                                std::cout << "E";
                            }
                            else
                            {
                                std::cout << "X";
                            }
                        }
                        std::cout << std::endl;
                        std::cout << "[Symbole] = [Regle] [Regle] ... [Regle]" << std::endl;
                        std::cout << "[Regle] a pour format : [Symb] [Symb] [Symb]" << std::endl;
                        std::cout << "Syntaxe incorrecte." << std::endl;
                        return false;
                    }
                }
            }

        }

        std::cout << "==== Lecture de [ " << left[0] << " ] ====" << std::endl;

        if (rules[0][0] == left[0] && rules[0].size() != 1)
        {
            std::cout << "Recursivite a gauche detectee!" << std::endl;
            std::cout << "Les regles du symbole [ " << left[0] << " ] seront modifiees." << std::endl;
            std::cout << "Le symbole [ " << left[0] << "' ] sera cree." << std::endl;

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


            Symbol *symb = new Symbol(left[0]);
            Symbol *new_symb = new Symbol(left[0] + "'");

            for (unsigned int i = 1; i < rules.size(); i++)
            {
                std::vector<std::string> temp_symb;

                for (unsigned int j = 0; j < rules[i].size(); j++)
                {
                    temp_symb.push_back(rules[i][j]);
                }

                temp_symb.push_back(left[0] + "'");

                symb->addRule(temp_symb);
            }


            std::vector<std::string> temp_new_symb_rule_left;
            std::vector<std::string> temp_new_symb_rule_right;

            for (unsigned int i = 0; i < cloned_first_rule.size(); i++)
            {
                temp_new_symb_rule_left.push_back(cloned_first_rule[i]);
            }

            temp_new_symb_rule_left.push_back(left[0] + "'");
            temp_new_symb_rule_right.push_back("#");

            new_symb->addRule(temp_new_symb_rule_left);
            new_symb->addRule(temp_new_symb_rule_right);

            _grammar[left[0]] = symb;
            _orderedSymbols.push_back(left[0]);

            _grammar[left[0] + "'"] = new_symb;
            _orderedSymbols.push_back(left[0] + "'");

        }
        else
        {

            std::cout << "Pas de recursivite a gauche detectee." << std::endl;

            Symbol *symb = new Symbol(left[0]);

            for (unsigned int i = 0; i < rules.size(); i++)
            {
                symb->addRule(rules[i]);
            }

            _grammar[left[0]] = symb;
            _orderedSymbols.push_back(left[0]);

        }

        std::cout << "------------------------" << std::endl;


    }

    input.close();

    return true;
}
