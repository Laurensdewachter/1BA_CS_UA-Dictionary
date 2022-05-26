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

public:
    NFA();
    NFA(string filename);
    bool accepts(string String);
    void print();
    string bool_as_text(bool b);
    DFA toDFA();
};


#endif //DICTIONARY_NFA_H
