//
// Created by robbe on 01.06.22.
//

#ifndef DICTIONARY_WORDLE_H
#define DICTIONARY_WORDLE_H
#include <vector>
#include <iostream>
#include "Woordenboek.h"


bool isAccepted(std::string word);


class Wordle {
private:
    std::vector<std::string> guesses;
    unsigned int numGuesses;
    std::string word;
    unsigned int wordLength;
    std::vector<std::vector<std::string>> colors;
    Woordenboek* w;
public:
/**
 * \Pre: er is een bestaand woordenboek met enkel woorden van lengte n;
 * \Post: er is een Wordle aangemaakt en uitgeprint, klaar om te spelen!!!
 */
    Wordle(Woordenboek* w, unsigned int numGuesses = 6);

/**
 * \Pre: De wordle is correct aangemaakt
 * \Post: De momentele staat van het spel is uitgeprint, clears terminal
 */
    void print();

/**
 * \Pre: De wordle is correct aangemaakt
 * \Post: het spel wordt geupdate en uitgeprint.
 */
    void guesseWord(std::string guess);

/**
 * \Pre: De wordle is correct aangemaakt
 * \Post: De vooruitgang wordt verwijdert, er wordt een nieuw woord gekozen en het spel wordt uigeprint
 */
    void clear();
};


#endif //DICTIONARY_WORDLE_H
