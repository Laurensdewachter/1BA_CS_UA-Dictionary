#include "Woordenboek.h"

Woordenboek::Woordenboek(){
    boek.setStartingState("Start");
    boek.setCurrentState("Start");
}

Woordenboek::Woordenboek(const vector<string> &woorden, const NFA &boek) : woorden(woorden), boek(boek) {}

Woordenboek::Woordenboek(const string &listName) {
    if(listName.size()>5){
        string splitsing=".";
        string extension=listName.substr(listName.find(splitsing),listName.size());
        if(extension=="json"){
            boek= NFA(listName);
            woorden=boek.getFinalStates();
            return;
        }
    }
    boek.setStartingState("Start");
    boek.setCurrentState("Start");
    ifstream file(listName);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::transform(line.begin(), line.end(), line.begin(), [](unsigned char c){ return std::tolower(c);});
            addWoord(line);
            boek.setCurrentState("Start");
        }
        file.close();
    }
}

void Woordenboek::operator=(Woordenboek &newDict) {
    woorden = newDict.woorden;
    boek = newDict.boek;
}

Woordenboek::~Woordenboek() {}

void Woordenboek::addWoord(const string woord) {
    string totalString;
    string currString;
    bool final = false;
    for(char letter:woord){
        totalString.push_back(letter);
        if (totalString == woord) {
            final = true;
        }
        currString = letter;
        boek.addState(boek.getCurrentState(), totalString, currString, final);
        boek.setCurrentState(totalString);
    }
    woorden.push_back(woord);
    boek.setCurrentState("Start");
}

void Woordenboek::removeWoord(const std::string &woord, bool removeUnreachable) {
    if (boek.accepts(woord)) {
        std::string state = boek.getCurrentState();
        std::vector<std::string> fs = boek.getFinalStates();
        for (std::vector<std::string>::iterator itfs = fs.begin(); itfs != fs.end(); itfs++) {
            if ((*itfs) == state) {
                fs.erase(itfs);
                boek.setFinalStates(fs);
                break;
            }
        }
        for (std::vector<std::string>::iterator itw = woorden.begin(); itw != woorden.end(); itw++) {
            if ((*itw) == woord) {
                woorden.erase(itw);
                break;
            }
        }
        boek.minimizeUnreachable(woord);
    }
    if (removeUnreachable)
        boek.removeUnreachable();
}

void Woordenboek::minimaliseer() {
    boek.removeUnreachable();
}

void Woordenboek::controleer(const std::string &inFileName) {
    size_t startExtension = inFileName.find_last_of('.');
    string outFileName = inFileName.substr(0, startExtension);
    outFileName += "_corrected.html";
    checkText(inFileName.c_str(), outFileName.c_str());
}

void Woordenboek::getWoordenboekVanLengte(unsigned int woordLengte) {
    Woordenboek woordenboekLengte = Woordenboek();
    vector<string> woordenWeg;
    for (const auto &woord: woorden) {
        if (woord.size() == woordLengte)
            woordenboekLengte.woorden.push_back(woord);
        else
            woordenWeg.push_back(woord);
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
    for (const auto &woord: woordenWeg) {
        removeWoord(woord, false);
    }
    boek.removeUnreachable();
    *this = woordenboekLengte;
}

void Woordenboek::save(std::ostream &onstream) {
    for (auto word : woorden) {
        onstream << word << "\n";
    }
}

void Woordenboek::combineer(Woordenboek dict2) {
    NFA product(boek, dict2.boek, false);
    pushWords(dict2.woorden);
}

string Woordenboek::giveSuggestion(const std::string &letters) {
    boek.makeStochastic(woorden);
    string str = boek.getSuggestion(letters);
    return str;
}

void Woordenboek::pushWords(vector<string> words) {
    for (auto i:words){
        auto result = std::find(woorden.begin(), woorden.end(), i);
        if(result == woorden.end()) {
            woorden.push_back(i);
        }
    }
}

void Woordenboek::gemeenschappelijk(Woordenboek dict2) {
    vector<string> newWoorden;
    NFA product(boek, dict2.boek, true);
    for(auto i:woorden){
        if(find(dict2.woorden.begin(), dict2.woorden.end(), i) != dict2.woorden.end()){
            newWoorden.push_back(i);
        }
    }
    woorden = newWoorden;
    boek = product;
}

void Woordenboek::checkText(const char *inputFile, const char *outputFile) { //Alle leestekens worden nu nog naar spatie's omgezet
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);
    if (not inFile.is_open() || not outFile.is_open()) {
        return;
    }
    string line;
    regex wordSearch = regex("\\w+");
    smatch wordMatch;
    while (getline(inFile, line)) {
        string restLine = line;
        while (regex_search(restLine, wordMatch, wordSearch)) {
            string word = wordMatch.str();
            if (not boek.accepts(word)) {
                regex wordRegex = regex('('+word+')');
                line = regex_replace(line, wordRegex, "<b style=\"color:red\"> $1 </b>");
            }
            restLine = wordMatch.suffix();
        }
        outFile << line << "<br>" << '\n';
    }
    inFile.close();
    outFile.close();
}

bool Woordenboek::accepts(const std::string &word) {
    return boek.accepts(word);
}
