bool windows = false;
#ifdef _WIN32
    windows = true;
#endif
#ifdef _WIN64
    windows = true;
#endif

#include <iostream>
#include "Shell.h"
#include "Woordenboek.h"

int main() {
    Shell cli(windows);
    cli.launch();

    Woordenboek w = Woordenboek("../abc.txt");
    Woordenboek w1 = Woordenboek("../Woordenlijst.txt");
    cout << w.giveSuggestion("Ab")<<endl;
    Woordenboek w2 = Woordenboek::combineer(w, w1);
    cout << w2.giveSuggestion("")<< endl;
    cout << w2.giveSuggestion("b")<< endl;
    cout << w2.giveSuggestion("m")<< endl;
    cout << w2.giveSuggestion("o")<< endl;
    cout << w2.giveSuggestion("O")<< endl;
    Woordenboek w3 = Woordenboek::gemeenschappelijk(w,w1);
   cout << w3.giveSuggestion("") << endl;
    return 0;
}
