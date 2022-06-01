#ifndef DICTIONARY_WOORDENBOEK_H
#define DICTIONARY_WOORDENBOEK_H

#include <vector>
#include <string>
#include <iostream>
#include "NFA.h"

using namespace std;

class Woordenboek {
private:
    vector<string> woorden;
    NFA boek;
public:
    /**
     * \Post: leeg woordenboek aangemaakt
     */
    Woordenboek();

    Woordenboek(const vector<string> &woorden, const NFA &boek);

    /**
     * \Pre: Er is een woordenlijst om woordenboek op te baseren
     * \Pre: Woordenboek aangemaakt met woorden van woordenlijst erin
     */
    explicit Woordenboek(const string &listName);

    ~Woordenboek();

    /**
     * \Pre: Er bestaat een woordenboek
     * \Post: Het woord is toegevoegd aan het woordenboek
     */
    void addWoord(const string woord);

    /**
     * \brief slaag het woordenboek op
     * \author Laurens De Wachter
     * @param onstream de stream waarop het woordenboek geschreven zal worden
     */
    void save(std::ofstream &onstream);

    void pushWords(vector<string> words);

    /**
     * \Pre: Er bestaat een niet leeg woordenboek
     * \Post: NFA boek is geminimaliseerd
     */
    void minimaliseer();

    /**
     * \pre: Er bestaan twee niet lege woordenboeken
     * \Post: Er bestaat een nieuwe NFA (woordenboek) die de combinatie is van de twee gegeven woordenboeken
     */
    static Woordenboek combineer(Woordenboek dict1, Woordenboek dict2);

    /**
     * \Pre: Er bestaat een niet lege woordenboeken
     *
     * \return: Het woord dat het meest waarschijnlijke is om getypt te worden.
     */
    string giveSuggestion(string letters);

    /**
     * \Pre: Er bestaan twee niet lege woordenboeken
     * \Post: De gemeenschappelijke woorden van de twee woordenboeken zijn samengevoegd in één woordenboek.
     * \return: Het nieuw gevormde woordenboek.
     */
    static Woordenboek gemeenschappelijk(Woordenboek dict1, Woordenboek dict2);

    /**
     * \Pre: woordLengte i niet 0
     *
     * \return: een woordenboek met woorden van lengte n
     */
    Woordenboek getWoordenboekVanLengte(unsigned int woordLengte);

    void checkText(const char *inputFile, const char *outputFile);
};




#endif
