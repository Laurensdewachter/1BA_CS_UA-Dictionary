//
// Created by dante on 25/05/22.
//

#include "Woordenboek.h"

Woordenboek::Woordenboek(){}

Woordenboek::Woordenboek(const string listName) {
    boek.setStartingState("Start");
    boek.setCurrentState("Start");
    std::ifstream file(listName);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            addWoord(line);
        }
        file.close();
    }
}

void Woordenboek::addWoord(string woord) {
    string totalString;
    string currString;
    for(char letter:woord){
        totalString.push_back(letter);
        currString.push_back(letter);
        bool final = false;
        if (letter == woord.back()) {
            final = true;
        }
        boek.addState(boek.getCurrentState(), totalString, currString, final);
        boek.setCurrentState(totalString);
        currString="";
    }
}

void Woordenboek::minimaliseer() {

}

NFA Woordenboek::combineer(NFA dict1, NFA dict2) {
    NFA product(dict1, dict2, false);
    return product;
}
