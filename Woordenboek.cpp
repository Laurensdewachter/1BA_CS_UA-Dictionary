//
// Created by dante on 25/05/22.
//

#include "Woordenboek.h"

Woordenboek::Woordenboek(){}



Woordenboek::Woordenboek(const string listName) {
    boek.setStartingState("Start");
    boek.setCurrentState("Start");
    ifstream file(listName);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            addWoord(line);
            boek.setCurrentState("Start");
            woorden.push_back(line);
        }
        file.close();
    }
}

void Woordenboek::addWoord(const string woord) {
    string totalString;
    string currString;
    bool final = false;
    for(char letter:woord){
        totalString.push_back(letter);
        currString.push_back(letter);
        if (totalString == woord) {
            final = true;
        }
        boek.addState(boek.getCurrentState(), totalString, currString, final);
        boek.setCurrentState(totalString);
        currString="";
    }
}

void Woordenboek::minimaliseer() {

}

Woordenboek Woordenboek::combineer(Woordenboek dict1, Woordenboek dict2) {
    NFA product(dict1.boek, dict2.boek, false);
    dict1.pushWords(dict2.woorden);
    return Woordenboek(dict1.woorden, product);
}

string Woordenboek::giveSuggestion(string letters) {
    boek.makeStochastic(woorden);
    string str = boek.getSuggestion(letters);
    return str;
}

Woordenboek::Woordenboek(const vector<string> &woorden, const NFA &boek) : woorden(woorden), boek(boek) {}

void Woordenboek::pushWords(vector<string> words) {
    for (auto i:words){
        auto result = std::find(woorden.begin(), woorden.end(), i);
        if(result == woorden.end()) {
            woorden.push_back(i);
        }
    }
}

Woordenboek Woordenboek::gemeenschappelijk(Woordenboek dict1, Woordenboek dict2) {
    vector<string> woorden;
    NFA product(dict1.boek, dict2.boek, true);
    for(auto i:dict1.woorden){
        if(find(dict2.woorden.begin(), dict2.woorden.end(), i) != dict2.woorden.end()){
            woorden.push_back(i);
        }
    }
    return Woordenboek(woorden, product);
}
