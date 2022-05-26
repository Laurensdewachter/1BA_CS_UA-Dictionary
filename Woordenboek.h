//
// Created by dante on 25/05/22.
//

#ifndef DICTIONARY_WOORDENBOEK_H
#define DICTIONARY_WOORDENBOEK_H

#include <vector>
#include <string>
#include "NFA.h"

using namespace std;

class Woordenboek {
private:
    vector<string> woorden;
    NFA boek;
public:
    /*
     * Postconditie: leeg woordenboek aangemaakt
     */
    Woordenboek();

    /*
     * Preconditie: Er is een woordenlijst om woordenboek op te baseren
     * Postcondtie: Woordenboek aangemaakt met woorden van woordenlijst erin
     */
    Woordenboek(const string listName);

    /*
     * Pre: Er bestaat een woordenboek
     * Post: Het woord is toegevoegd aan het woordenboek
     */
    void addWoord(const string woord);

    /*
     * Pre: Er bestaat een niet leeg woordenboek
     * Post: NFA boek is geminimaliseerd
     */
    void minimaliseer();

    /*
     * Pre: Er bestaan twee niet lege woordenboeken
     * Post: Er bestaat een nieuwe NFA (woordenboek) die de combinatie is van de twee gegeven woordenboeken
     */
    NFA combineer(NFA dict1, NFA dict2);
};


#endif //DICTIONARY_WOORDENBOEK_H
