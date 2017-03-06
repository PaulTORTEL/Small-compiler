#include <iostream>
#include "Analyser.h"

using namespace std;

int main()
{
    Analyser analyser;
    if (analyser.readGrammar("grammars/test2.txt")) {
        analyser.displayGrammar();
        analyser.setAllFirst();
        analyser.displayFirst();
        analyser.setAllFollow();
        analyser.displayFollow();
        analyser.createTable();
        analyser.displayTable();
    }
    return 0;
}
