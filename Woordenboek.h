#ifndef DICTIONARY_WOORDENBOEK_H
#define DICTIONARY_WOORDENBOEK_H

#include <vector>
#include <string>
#include <iostream>
#include <regex>
#include "NFA.h"

using namespace std;

class Woordenboek {
public:
    vector<string> woorden;
private:
    NFA boek;
public:
//=============Constructors/Destructor=====================
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

    void operator=(Woordenboek &newDict);

    ~Woordenboek();
//=============Constructors/Destructor=====================

    /**
     * \Pre Er bestaat een woordenboek
     * \Post Het woord is toegevoegd aan het woordenboek
     */
    void addWoord(const string woord);

    /**
     * \brief Verwijder een woord
     * 
     */
    void removeWoord(const std::string &woord);

    /**
     * \Pre: Er bestaat een niet leeg woordenboek
     * \Post: NFA boek is geminimaliseerd
     */
    void minimaliseer();

    /**
     * \brief Controleer een tekst in een tekstbestand
     * \param fileName De file waar de tekst die gecontroleerd moet worden zich bevindt
     */
    void controleer(const std::string &fileName);

    /**
     * \Pre: woordLengte i niet 0
     * \return: een woordenboek met woorden van lengte n
     */
    void getWoordenboekVanLengte(unsigned int woordLengte);

    /**
     * \Pre: Er bestaat een niet lege woordenboeken
     *
     * \return: Het woord dat het meest waarschijnlijke is om getypt te worden.
     */
    string giveSuggestion(const std::string &letters);

    /**
     * \Pre: Er bestaan twee niet lege woordenboeken
     * \Post: De gemeenschappelijke woorden van de twee woordenboeken zijn samengevoegd in één woordenboek.
     * \return: Het nieuw gevormde woordenboek.
     */
    static Woordenboek gemeenschappelijk(Woordenboek dict1, Woordenboek dict2);

    /**
     * \brief slaag het woordenboek op
     * @param onstream de stream waarop het woordenboek geschreven zal worden
     */
    void save(std::ostream &onstream);

    /**
     * \pre: Er bestaan twee niet lege woordenboeken
     * \Post: Er bestaat een nieuwe NFA (woordenboek) die de combinatie is van de twee gegeven woordenboeken
     */
    void combineer(Woordenboek dict2);

    void checkText(const char *inputFile, const char *outputFile);

    bool accepts(const std::string &word);

private:
    void pushWords(vector<string> words);
};




#endif
