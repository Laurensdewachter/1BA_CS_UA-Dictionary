//
// Created by dante on 25/05/22.
//

#ifndef DICTIONARY_DFA_H
#define DICTIONARY_DFA_H

#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include "json.hpp"
#include "RE.h"

using nlohmann::json;

using namespace std;

class DFA {
    std::string type;
    std::string StartingState;
    std::string CurrentState;
    std::vector<std::string> FinalStates;
    std::vector<std::string> alphabet;
    std::map<std::string,std::vector<std::vector<std::string>>> transitions;

public:
    DFA();
    DFA(string filename);
    DFA(DFA dfa1, DFA dfa2, bool b);

    DFA(const string &type, const string &startingState, const vector<string> &finalStates,
        const vector<string> &alphabet, const map<string, vector<vector<string>>> &transitions);

    bool accepts(string String);
    void print();
    string bool_as_text(bool b);

    const string &getStartingState() const;

    void setStartingState(const string &startingState);

    const vector<string> &getFinalStates() const;

    void setFinalStates(const vector<string> &finalStates);

    const vector<string> &getAlphabet() const;

    void setAlphabet(const vector<string> &alphabet);

    const map<string, vector<vector<string>>> &getTransitions() const;

    void setTransitions(const map<string, vector<vector<string>>> &transitions);

    void finalPush(string str);

    const string &getType() const;

    void setType(const string &type);

    vector<string> pushalf(vector<string> alf);

    RE toRE();

    string FSTORE(string FS);

    string makeString(string R, string S, string U, string T);
};


#endif //DICTIONARY_DFA_H
