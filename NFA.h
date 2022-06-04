//
// Created by dante on 25/05/22.
//

#ifndef DICTIONARY_NFA_H
#define DICTIONARY_NFA_H


#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "json.hpp"
#include <map>
#include "DFA.h"
using nlohmann::json;

using namespace std;

class DFA;
class NFA {
    string type;
    string StartingState;
    string CurrentState;
    vector<string> FinalStates;
    vector<string> alphabet;
    map<string,vector<vector<string>>> transitions;
    map<string,vector<vector<string>>> stochasticTransitions;

public:
    NFA();
    NFA(string filename);
    NFA(NFA nfa1, NFA nfa2, bool b);
    bool accepts(string String);
    void print(std::ostream &onstream);
    string bool_as_text(bool b);
    DFA toDFA();

    const string &getType() const;

    void setType(const string &type);

    const string &getStartingState() const;

    void setStartingState(const string &startingState);

    const vector<string> &getFinalStates() const;

    void setFinalStates(const vector<string> &finalStates);

    const vector<string> &getAlphabet() const;

    void setAlphabet(const vector<string> &alphabet);

    const map<string, vector<vector<string>>> &getTransitions() const;

    void setTransitions(const map<string, vector<vector<string>>> &transitions);

    const string &getCurrentState() const;

    void setCurrentState(const string &currentState);

    void pushalf(vector<string> alf);

    void makeStochastic(vector<string> &woorden);

    void addState(string from, string to, string transition, bool final);

    void makeBranchStoch(string letters, vector<string> woorden, int number);

    string getSuggestion(string letters, bool b = true);

    void pushFinal(vector<string> final);

    void removeUnreachable();
};


#endif //DICTIONARY_NFA_H
