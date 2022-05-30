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

Woordenboek Woordenboek::getWoordenboekVanLengte(unsigned int woordLengte) {
    Woordenboek woordenboekLengte = Woordenboek();
    for (const auto &woord: woorden) {
        if (woord.size() == woordLengte)
            woordenboekLengte.woorden.push_back(woord);
    }
    auto transition = boek.getTransitions();
    auto finalstates = boek.getFinalStates();
    vector<string> curStates = {boek.getStartingState()};
    unsigned int i = 0;
    while (i != woordLengte) {
        i++;
        vector<string> newStates;
        for (const auto &preState: curStates) {     // Voor elke state
            for (const auto &nextTransition: transition[preState]) {    // voor elke letter
                for (int j = 1; j < nextTransition.size(); ++j) {       // voor elke transitie van een letter
                    auto finalstateIterator = find(finalstates.begin(), finalstates.end(), nextTransition[j]);
                    if (finalstateIterator != finalstates.end() &&
                        i != woordLengte) {     // als er een final state is met een kleinere lengte dan woordLengte
                        finalstates.erase(finalstateIterator);  // verwijder dan deze uit finalstates
                    } else {
                        newStates.push_back(nextTransition[j]);
                    }
                }
            }
        }
        curStates = newStates;
    }
    for (const auto &curState: curStates) {
        transition.erase(curState);
    }
    auto newBoek = NFA(boek);
    newBoek.setTransitions(transition);
    newBoek.setFinalStates(finalstates);
    newBoek.removeUnreachable();
    woordenboekLengte.boek = newBoek;
    woordenboekLengte.minimaliseer();
    return woordenboekLengte;
}

void Woordenboek::checkText(const char *inputFile, const char *outputFile) { //Alle leestekens worden nu nog naar spatie's omgezet
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);
    if (not inFile.is_open() || not outFile.is_open()) {
        return;
    }
    string line;
    char *token;
    while (getline(inFile, line)) {
        char *cline = &line[0];
        token = strtok(cline, " ,.!?");
        while (token != NULL) {
            if (not boek.accepts(token)) {
                outFile << "<b style=\"color:red\">" << token << "</b>";
            } else {
                outFile << token;
            }
            token = strtok(NULL, " ,.!?");
            if (token != NULL)
                outFile << ' ';
        }
        outFile << '\n';
    }
    inFile.close();
    outFile.close();
}
