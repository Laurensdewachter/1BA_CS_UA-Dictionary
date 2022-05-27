//
// Created by dante on 25/05/22.
//

#include "NFA.h"
#include <iostream>

NFA::NFA() {
    transitions={};
    alphabet={};
}

NFA::NFA(string filename) {
    ifstream input(filename);

    json j;
    input >> j;
    // tonen op het scherm

    type = j["type"];
    for(auto i:j["alphabet"]){
        alphabet.push_back(i);
    }
    for(auto i:j["states"]){
        if (i["starting"] == true){
            StartingState = string(i["name"]);
            CurrentState = string(i["name"]);
        }
        if (i["accepting"] == true){
            FinalStates.push_back(i["name"]);
        }
    }
    for(auto i:j["transitions"]){
        transitions[i["from"]].push_back({i["input"],i["to"]});
    }
}
bool NFA::accepts(string String) {
    for (auto i:String) {
        for (auto s:transitions[CurrentState]) {
            if(s[0] == string(1,i)){
                CurrentState = s[1];
                break;
            }
        }
    }
    std::vector<string>::iterator it;
    it = std::find(FinalStates.begin(), FinalStates.end(), CurrentState);
    if (it != FinalStates.end()){
        return true;
    }
    else{
        return false;
    }
}

void NFA::print() {
    vector<map<string,nlohmann::json>> states;
    for (auto i:transitions){
        nlohmann::json state;
        state["name"] = i.first; //string
        state["starting"] = (i.first == StartingState); //bool
        std::vector<string>::iterator it;
        it = std::find(FinalStates.begin(), FinalStates.end(), i.first);
        if(it != FinalStates.end()){
            state["accepting"] = true; //bool
        }
        states.push_back(state);
    }
    vector<map<string,string>> Transitions;
    for(auto i:transitions){
        map<string, string> transition;
        for(auto j:i.second){
            transition["from"] =  i.first;
            transition["to"] = j[1];
            transition["input"] = j[0];

            Transitions.push_back(transition);
        }
    }


    json j{
            {"type", type},
            {"alphabet", alphabet},
            {"states", states},
            {"transitions", Transitions}
    };
    cout << setw(4) << j << endl;
}

//source function = https://stackoom.com/en/question/7dv
string NFA::bool_as_text(bool b) {
    if (b){
        return "true";
    }
    else{
        return "false";
    }

}

DFA NFA::toDFA() {
    DFA dfa = DFA();
    dfa.setType("DFA");
    dfa.setAlphabet(alphabet);
    dfa.setStartingState("{" + StartingState + "}");
    map<string,vector<vector<string>>> t;
    map<string, vector<vector<string>>> oldmap;
    int oldsize = 1;
    for (auto j: alphabet){
        string str = "{";
        for (auto k:transitions[StartingState]){
            if (k[0] == j){
                str += k[1] + ",";
            }
        }
        if (str != "{"){
            str.pop_back();
        }
        str += "}";
        t["{" + StartingState + "}"].push_back({j, str});

        if (t.find(str) == t.end()){
            t[str];
        }
    }
    oldmap["{" + StartingState + "}"] = t["{" + StartingState + "}"];
    map<string, vector<vector<string>>> newmap = t;
    int newsize = t.size();
    while (oldsize != newsize){
        oldsize = t.size();
        for (auto i:newmap){
            if (oldmap.find(i.first) == oldmap.end()){
                string str = i.first.substr(1, i.first.size()-2);

                stringstream ss(str);
                vector<string> states;

                while (ss.good()) {
                    string substr;
                    getline(ss, substr, ',');
                    states.push_back(substr);
                }

                for (auto l: alphabet){
                    vector<string> strvec;
                    for (auto j: states){
                        vector<vector<string>> k = transitions[j];
                        for (auto m:k){
                            if (m[0] == l) {
                                std::vector<string>::iterator it;
                                it = std::find(strvec.begin(), strvec.end(), m[1]);
                                if (it == strvec.end()) {
                                    strvec.push_back(m[1]);
                                }
                            }
                        }
                    }
                    sort(strvec.begin(), strvec.end());
                    std::ostringstream vts;
                    if (!strvec.empty())
                    {
                        std::copy(strvec.begin(), strvec.end()-1,
                                  std::ostream_iterator<string>(vts, ","));
                        vts << strvec.back();
                    }
                    string string1 = "{" + vts.str() + "}";
                    t[i.first].push_back({l, string1});
                    if (t.find(string1) == t.end()){
                        t[string1];
                    }
                }
            }
        }
        oldmap = newmap;
        newmap = t;
        newsize = t.size();
    }
    dfa.setTransitions(t);
    for (auto i:t){
        string str = i.first.substr(1, i.first.size()-2);

        stringstream ss(str);
        vector<string> states;

        while (ss.good()) {
            string substr;
            getline(ss, substr, ',');
            states.push_back(substr);
        }
        for (auto j:states){
            std::vector<string>::iterator it;
            it = std::find(FinalStates.begin(), FinalStates.end(), j);
            if(it != FinalStates.end()){
                dfa.finalPush(i.first);
                break;
            }
        }
    }
    return dfa;
}

NFA::NFA(NFA nfa1, NFA nfa2, bool b) {
    type = "DFA";
    alphabet = (nfa1.getAlphabet());
    pushalf(nfa2.getAlphabet());
    StartingState ="(" + nfa1.getStartingState() + "," + nfa2.getStartingState() + ")";
    map<string,vector<vector<string>>> t;
    t["(" + nfa1.getStartingState() + "," + nfa2.getStartingState() + ")"] = {};
    map<string, vector<vector<string>>> oldmap = t;
    int oldsize = 1;
    for (auto j: alphabet){
        string str = "(";
        vector<string> state;
        map<string, vector<vector<string>>> tra = nfa1.getTransitions();
        for (auto i:tra[nfa1.getStartingState()]){
            if (i[0] == j){
                state.push_back(i[1]);
                break;
            }
        }
        tra = nfa2.getTransitions();
        for (auto i:tra[nfa2.getStartingState()]){
            if (i[0] == j){
                state.push_back(i[1]);
                break;
            }
        }
        str = "(" + state[0] + "," + state[1] + ")";
        t["(" + nfa1.getStartingState() + "," + nfa2.getStartingState() + ")"].push_back({j, str});
        if (t.find(str) == t.end()){
            t[str];
        }
    }
    map<string, vector<vector<string>>> newmap = t;
    int newsize = t.size();
    while (oldsize != newsize){
        oldsize = t.size();
        for (auto i:newmap) {
            if (oldmap.find(i.first) == oldmap.end()) {
                string str = i.first.substr(1, i.first.size() - 2);

                stringstream ss(str);
                vector<string> states;

                while (ss.good()) {
                    string substr;
                    getline(ss, substr, ',');
                    states.push_back(substr);
                }
                vector<string> strvec;
                for (auto l: alphabet) {
                    vector<string> state;
                    map<string, vector<vector<string>>> tra = nfa1.getTransitions();
                    for (auto i: tra[states[0]]) {
                        if (i[0] == l) {
                            state.insert(state.begin(), i[1]);
                        }
                    }
                    tra = nfa2.getTransitions();
                    for (auto i: tra[states[1]]) {
                        if (i[0] == l) {
                            state.push_back(i[1]);
                        }
                    }
                    str = "(" + state[0] + "," + state[1] + ")";
                    t[i.first].push_back({l, str});
                    if (t.find(str) == t.end()) {
                        t[str];
                    }
                }
            }
        }
        oldmap = newmap;
        newmap = t;
        newsize = t.size();
    }
    transitions = t;
    for (auto i:t){
        string str = i.first.substr(1, i.first.size()-2);

        stringstream ss(str);
        vector<string> states;

        while (ss.good()) {
            string substr;
            getline(ss, substr, ',');
            states.push_back(substr);
        }
        if (b){
            int teller = 0;
            for (auto j:states){

                if (nfa1.getTransitions().find(j) != nfa1.getTransitions().end()) {
                    auto result = std::find(nfa1.getFinalStates().begin(), nfa1.getFinalStates().end(), j);
                    if(result != nfa1.getFinalStates().end()){
                        teller += 1;
                    }
                }
                else {
                    auto result = std::find(nfa2.getFinalStates().begin(), nfa2.getFinalStates().end(), j);
                    if(result != nfa2.getFinalStates().end()){
                        teller += 1;
                    }
                }
            }
            if(teller == 2){
                FinalStates.push_back(i.first);
            }
        }
        if (!b){
            for (auto j:states){
                if (nfa1.getTransitions().find(j) != nfa1.getTransitions().end()) {
                    auto result = std::find(nfa1.getFinalStates().begin(), nfa1.getFinalStates().end(), j);
                    if(result != nfa1.getFinalStates().end()){
                        FinalStates.push_back(i.first);
                        break;
                    }
                }
                else {
                    auto result = std::find(nfa2.getFinalStates().begin(), nfa2.getFinalStates().end(), j);
                    if(result != nfa2.getFinalStates().end()){
                        FinalStates.push_back(i.first);
                        break;
                    }
                }
            }
        }
    }
}

const string &NFA::getType() const {

}

void NFA::setType(const string &type) {
    NFA::type = type;
}

const string &NFA::getStartingState() const {
    return StartingState;
}

void NFA::setStartingState(const string &startingState) {
    StartingState = startingState;
}

const vector<string> &NFA::getFinalStates() const {
    return FinalStates;
}

void NFA::setFinalStates(const vector<string> &finalStates) {
    FinalStates = finalStates;
}

const vector<string> &NFA::getAlphabet() const {
    return alphabet;
}

void NFA::setAlphabet(const vector<string> &alphabet) {
    NFA::alphabet = alphabet;
}

map<string, vector<vector<string>>> &NFA::getTransitions() {
    return transitions;
}

void NFA::setTransitions(const map<string, vector<vector<string>>> &transitions) {
    NFA::transitions = transitions;
}

const string &NFA::getCurrentState() const {
    return CurrentState;
}

void NFA::setCurrentState(const string &currentState) {
    CurrentState = currentState;
}

vector<string> NFA::pushalf(vector<string> alf) {
    for (auto i:alf){
        auto result = std::find(alphabet.begin(), alphabet.end(), i);
        if(result == alphabet.end()) {
            alphabet.push_back(i);
        }
    }
    return alphabet;
}

void NFA::addState(string from, string to, string transition, bool final) {
    if(find(alphabet.begin(),alphabet.end(),transition)==alphabet.end()){
        alphabet.push_back(transition);
    }
    if(final){
        FinalStates.push_back(to);
    }
    if(transitions.find(from)!=transitions.end()){
        transitions.find(from)->second.push_back({transition,to});
    }
    else{
        transitions[from]={{transition,to}};
    }
}


