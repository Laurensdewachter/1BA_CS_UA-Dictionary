//
// Created by dante on 25/05/22.
//

#include "Woordenboek.h"

Woordenboek::Woordenboek(){}

Woordenboek::Woordenboek(const string listName) {

}

void Woordenboek::addWoord(const string woord) {

}

void Woordenboek::minimaliseer() {

}

NFA Woordenboek::combineer(NFA dict1, NFA dict2) {
    NFA product(dict1, dict2, false);
    return product;
}

string Woordenboek::giveSuggestion(string letters) {
    NFA s = NFA::make
    return std::string();
}
