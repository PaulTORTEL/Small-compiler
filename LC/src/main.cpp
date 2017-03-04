#include <iostream>
#include "Analyser.h"

using namespace std;

int main()
{
    Analyser analyser;
    if (analyser.readGrammar("grammars/test1.txt")) {
        analyser.displayGrammar();
        analyser.setAllFirst();
        analyser.displayFirst();
    }
    return 0;
}
