#include "Analyser.h"
#include <iostream>
#include <fstream>
#include <string>

bool Analyser::readGrammar(const char* filename)
{
    std::ifstream input( filename );

    for( std::string line; getline( input, line ); )
    {

    }

    input.close();

    return true;
}
