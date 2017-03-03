#include "Analyser.h"


bool Analyser::readGrammar(const char* filename)
{
    std::ifstream input( filename );

    for( std::string line; getline( input, line ); )
    {
        std::cout << line << std::endl;
    }

    input.close();

    return true;
}
