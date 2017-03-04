#include <iostream>
#include "Analyser.h"

using namespace std;

int main()
{
    Analyser analyser;
    if (analyser.readGrammar("grammars/test1.txt")) {
        analyser.displayRules();
    }
    return 0;
}
