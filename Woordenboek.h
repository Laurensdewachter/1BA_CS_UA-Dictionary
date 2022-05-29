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

    Woordenboek(const vector<string> &woorden, const NFA &boek);

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

    void pushWords(vector<string> words);

    /*
     * Pre: Er bestaat een niet leeg woordenboek
     * Post: NFA boek is geminimaliseerd
     */
    void minimaliseer();

    /*
     * Pre: Er bestaan twee niet lege woordenboeken
     * Post: Er bestaat een nieuwe NFA (woordenboek) die de combinatie is van de twee gegeven woordenboeken
     */
    static Woordenboek combineer(Woordenboek dict1, Woordenboek dict2);

    /*
     * Pre: Er bestaat een niet lege woordenboeken
     * Post: /
     * return: Het woord dat het meest waarschijnlijke is om getypt te worden.
     */
    string giveSuggestion(string letters);

    /*
     * Pre: woordLengte i niet 0
     * Post: /
     * return: een woordenboek met woorden van lengte n
     */
    Woordenboek getWoordenboekVanLengte(unsigned int woordLengte);


    static Woordenboek gemeenschappelijk(Woordenboek dict1, Woordenboek dict2);
};




#endif //DICTIONARY_WOORDENBOEK_H
