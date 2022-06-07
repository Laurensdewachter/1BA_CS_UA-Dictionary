//
// Created by dante on 25/05/22.
//

#include "DFA.h"
#include "NFA.h"
#include <iostream>

DFA::DFA() {}

DFA::DFA(string filename) {
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
            StartingState = i["name"];
            CurrentState = i["name"];
        }
        if (i["accepting"] == true){
            FinalStates.push_back(i["name"]);
        }
    }
    for(auto i:j["transitions"]){
        transitions[i["from"]].push_back({i["input"],i["to"]});
    }
}
bool DFA::accepts(string String) {
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
    if(it != FinalStates.end()){
        return true;
    }
    else{
        return false;
    }
}

void DFA::print() {
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
        else{
            state["accepting"] = false;
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
string DFA::bool_as_text(bool b) {
    if (b){
        return "true";
    }
    else{
        return "false";
    }

}

const string &DFA::getStartingState() const {
    return StartingState;
}

void DFA::setStartingState(const string &startingState) {
    StartingState = startingState;
}

const vector<string> &DFA::getAlphabet() const {
    return alphabet;
}

void DFA::setAlphabet(const vector<string> &alphabet) {
    DFA::alphabet = alphabet;
}

const map<string, vector<vector<string>>> &DFA::getTransitions() const {
    return transitions;
}

void DFA::setTransitions(const map<string, vector<vector<string>>> &transitions) {
    DFA::transitions = transitions;
}

const vector<string> &DFA::getFinalStates() const {
    return FinalStates;
}

void DFA::setFinalStates(const vector<string> &finalStates) {
    FinalStates = finalStates;
}

void DFA::finalPush(string str) {
    FinalStates.push_back(str);
}

const string &DFA::getType() const {
    return type;
}

void DFA::setType(const string &type) {
    DFA::type = type;
}

vector<string> DFA::pushalf(vector<string> alf) {
    for (auto i:alf){
        auto result = std::find(alphabet.begin(), alphabet.end(), i);
        if(result == alphabet.end()) {
            alphabet.push_back(i);
        }
    }
    return alphabet;
}

DFA::DFA(DFA dfa1, DFA dfa2, bool b) {
    type = "DFA";
    alphabet = (dfa1.getAlphabet());
    pushalf(dfa2.getAlphabet());
    StartingState ="(" + dfa1.getStartingState() + "," + dfa2.getStartingState() + ")";
    map<string,vector<vector<string>>> t;
    t["(" + dfa1.getStartingState() + "," + dfa2.getStartingState() + ")"] = {};
    map<string, vector<vector<string>>> oldmap = t;
    int oldsize = 1;
    for (auto j: alphabet){
        string str = "(";
        vector<string> state;
        map<string, vector<vector<string>>> tra = dfa1.getTransitions();
        for (auto i:tra[dfa1.getStartingState()]){
            if (i[0] == j){
                state.push_back(i[1]);
                break;
            }
        }
        tra = dfa2.getTransitions();
        for (auto i:tra[dfa2.getStartingState()]){
            if (i[0] == j){
                state.push_back(i[1]);
                break;
            }
        }
        str = "(" + state[0] + "," + state[1] + ")";
        t["(" + dfa1.getStartingState() + "," + dfa2.getStartingState() + ")"].push_back({j, str});
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
                    map<string, vector<vector<string>>> tra = dfa1.getTransitions();
                    for (auto i: tra[states[0]]) {
                        if (i[0] == l) {
                            state.insert(state.begin(), i[1]);
                        }
                    }
                    tra = dfa2.getTransitions();
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

                if (dfa1.getTransitions().find(j) != dfa1.getTransitions().end()) {
                    auto result = std::find(dfa1.getFinalStates().begin(), dfa1.getFinalStates().end(), j);
                    if(result != dfa1.getFinalStates().end()){
                        teller += 1;
                    }
                }
                else {
                    auto result = std::find(dfa2.getFinalStates().begin(), dfa2.getFinalStates().end(), j);
                    if(result != dfa2.getFinalStates().end()){
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
                if (dfa1.getTransitions().find(j) != dfa1.getTransitions().end()) {
                    auto result = std::find(dfa1.getFinalStates().begin(), dfa1.getFinalStates().end(), j);
                    if(result != dfa1.getFinalStates().end()){
                        FinalStates.push_back(i.first);
                        break;
                    }
                }
                else {
                    auto result = std::find(dfa2.getFinalStates().begin(), dfa2.getFinalStates().end(), j);
                    if(result != dfa2.getFinalStates().end()){
                        FinalStates.push_back(i.first);
                        break;
                    }
                }
            }
        }
    }
}

RE DFA::toRE() {
    RE re = RE();
    string strFSstr;
    vector<string> strFS;
    for(auto Finalstate:FinalStates){


        string str;
        map<string, vector<vector<string>>> tra = transitions;
        vector<string> stri1;
        string stri;
        for (auto i:tra[StartingState]){
            if (i[1] == StartingState){
                if(!stri1.empty()){
                    if(*find(stri1.begin(), stri1.end(), i[1]) != i[1]){
                        stri1.push_back(i[0]);
                    }
                }
                else{
                    stri1.push_back(i[0]);
                }
            }
        }
        sort(stri1.begin(), stri1.end());
        if (stri1.size() > 1) {
            stri = "(";
            for (int i=0; i<stri1.size()-1; i++){
                stri += stri1[i] + "+";
            }
            stri += stri1[stri1.size()-1] + ")";
        }
        else if(stri.size() == 1){
            stri = stri1[0];
        }
        if(!stri.empty()){
            for(int q=0; q<tra[StartingState].size(); q++){
                if (tra[StartingState][q][1] == StartingState){
                    tra[StartingState].erase(tra[StartingState].begin()+q);
                    q -= 1;
                }
            }
            tra[StartingState].push_back({stri, StartingState});
        }
        auto it = tra.cbegin();
        while (it != tra.cend()){
            if (it->first != Finalstate and it->first != StartingState) {
                vector<pair<string,string>> states;
                for (auto t: tra) {
                    if (t.first != it->first) {
                        for (auto u: t.second) {
                            if (u[1] == it->first) {
                                for (auto v: tra[it->first]) {
                                    if (v[1] != it->first) {
                                        if (states.size() > 0) {
                                            auto result = std::find(states.begin(), states.end(), pair<string, string>(t.first, v[1]));
                                            if (result != states.end()){
                                                continue;
                                            }
                                            else{
                                                states.push_back(pair<string, string>(t.first, v[1]));
                                            }
                                        }
                                        else {
                                            states.push_back(pair<string, string>(t.first, v[1]));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                for (auto j: states) {
                    vector<string> string10;
                    string string1 = "";
                    for (auto k: tra[j.first]) {
                        if (k[1] == it->first) {
                            if(!string10.empty()){
                                if(*find(string10.begin(), string10.end(), k[1]) != k[1]){
                                    string10.push_back(k[0]);
                                }
                            }
                            else{
                                string10.push_back(k[0]);
                            }
                        }
                    }
                    sort(string10.begin(), string10.end());
                    if (string10.size() > 1) {
                        string1 = "(";
                        for (int i=0; i<string10.size()-1; i++){
                            string1 += string10[i] + "+";
                        }
                        string1 += string10[string10.size()-1] + ")";
                    }
                    else{
                        string1 = string10[0];
                    }
                    vector<string> string20;
                    string string2 = "";
                    for (auto k: tra[it->first]) {
                        if (k[1] == it->first) {
                            if(!string20.empty()){
                                if(*find(string20.begin(), string20.end(), k[1]) != k[1]){
                                    string20.push_back(k[0]);
                                }}
                            else{
                                string20.push_back(k[0]);
                            }
                        }
                    }
                    if (string20.size() == 1) {
                        if(string20[0].size() == 1){
                            string2 = string20[0] + "*";
                        }
                        else{
                            string2 = "(" + string20[0] + ")*";
                        }
                    } else if (string20.size() > 1) {
                        sort(string20.begin(), string20.end());
                        string2 = "(";
                        for( int i=0; i<string20.size()-1; i++){
                            string2 += string20[i] + "+";
                        }
                        string2 += string20[string20.size()-1] + ")*";
                    }
                    vector<string> string30;
                    string string3 = "";
                    for (auto x: tra[it->first]) {
                        if (x[1] == j.second) {
                            if(!string30.empty()){
                                if(*find(string30.begin(), string30.end(), x[1]) != x[1]){
                                    string30.push_back(x[0]);
                                }}
                            else{
                                string30.push_back(x[0]);
                            }
                        }
                    }
                    sort(string30.begin(), string30.end());
                    if (string30.size() > 1) {
                        string3 = "(";
                        for (int i=0; i<string30.size()-1; i++){
                            string3 += string30[i] + "+";
                        }
                        string3 += string30[string30.size()-1] + ")";
                    }
                    else{
                        string3 = string30[0];
                    }
                    string string4 = string1 + string2 + string3;
                    vector<string> string40 = {string4};
                    for (auto b:tra[j.first]){
                        if (b[1] == j.second){
                            if(!string30.empty()){
                                if(*find(string40.begin(), string40.end(), b[1]) != b[1]){
                                    string40.push_back(b[0]);
                                }}
                            else{
                                string40.push_back(b[0]);
                            }
                        }}
                    if (string40.size()>1){
                        sort(string40.begin(), string40.end());
                        string4 = "(";
                        for (int i=0; i<string40.size()-1; i++){
                            string4 += string40[i] + "+";
                        }
                        string4 += string40[string40.size()-1] + ")";
                    }

                    for (int p=0; p<tra[j.first].size(); p++){
                        if(tra[j.first][p][1] == j.second){
                            tra[j.first].erase(tra[j.first].begin()+p);
                        }
                    }
                    tra[j.first].push_back({string4, j.second});
                }
                for(auto z:tra){
                    for(int q=0; q<z.second.size(); q++){
                        if (z.second[q][1] == it->first){
                            z.second.erase(z.second.begin()+q);
                            q -= 1;
                        }
                    }
                    tra[z.first] = z.second;
                }
                it = tra.erase(it);
            }
            else{
                it++;
            }
        }
        vector<string> strin;
        string strin1;
        if (!tra[Finalstate].empty()) {
            for (auto i: tra[Finalstate]) {
                if (i[1] == Finalstate) {
                    if (!strin.empty()) {
                        if (*find(strin.begin(), strin.end(), i[1]) != i[1]) {
                            strin.push_back(i[0]);
                        }
                    } else {
                        strin.push_back(i[0]);
                    }
                }
            }
            sort(strin.begin(), strin.end());
            if (strin.size() > 1) {
                strin1 += "(";
                for (int i = 0; i < strin.size() - 1; i++) {
                    strin1 += strin[i] + "+";
                }
                strin1 += strin[strin.size() - 1] + ")";
            } else if(strin.size() == 1){
                strin1 = strin[0];
            }
            if (!strin.empty()) {
                for (int q = 0; q < tra[Finalstate].size(); q++) {
                    if (tra[Finalstate][q][1] == Finalstate) {
                        tra[Finalstate].erase(tra[Finalstate].begin() + q);
                        q -= 1;
                    }
                }
                tra[Finalstate].push_back({strin1, Finalstate});
            }
        }
        string R;
        string S;
        string T;
        string U;
        string B;
        for (auto i: tra[StartingState]) {
            if (i[1] == StartingState) {
                B = i[0];
                if (StartingState == Finalstate) {
                    str += i[0] + "*";
                    strFS.push_back(str);
                    break;
                } else {
                    R = i[0];
                }
            } else {
                S = i[0];
            }
        }
        if(str == B+"*"){
            continue;
        }
        for (auto i: tra[Finalstate]) {
            if (i[1] == Finalstate) {
                U = i[0];
            }
        }
        for (auto i: tra[Finalstate]) {
            if (i[1] != Finalstate) {
                T = i[0];
            }
        }
        strFS.push_back(makeString(R, S, U, T));
    }
    sort(strFS.begin(), strFS.end());
    for(int i=0; i<strFS.size()-1; i++){
        strFSstr += strFS[i] + "+";
    }
    strFSstr += strFS[strFS.size()-1];
    re.setStr(strFSstr);
    return re;
}
DFA::DFA(const string &type, const string &startingState, const vector<string> &finalStates,
         const vector<string> &alphabet, const map<string, vector<vector<string>>> &transitions) : type(type),
                                                                                                   StartingState(
                                                                                                           startingState),
                                                                                                   FinalStates(
                                                                                                           finalStates),
                                                                                                   alphabet(alphabet),
                                                                                                   transitions(
                                                                                                           transitions) {}

string DFA::makeString(string R, string S, string U, string T) {
    string str;
    if(R.empty()){
        if (S.empty() or U.empty() or T.empty()){
            if (S.empty()){
                return "";
            }
            else if(U.empty() and !T.empty()){
                str = "(" + S + T + ")*" + S;
            }
            else if(U.empty()){
                str = S;
            }
            else{
                if((U[0] != '(' or U[U.size()-1] != ')') and U.size() > 1){
                    str = S + "(" + U + ")*";
                }
                else{
                    str = S + U + "*";
                }

            }
        }
        else{
            str = "(" + S + U + "*" + T + ")*" + S + U + "*";
        }
    }
    else{
        if (S.empty() or U.empty() or T.empty()){
            if(S.empty()){
                if (R[0] != '(' or R[R.size()-1] != ')' and R.size()>1){
                    if((U[0] != '(' or U[U.size()-1] != ')') and U.size() > 1){
                        str = "(" + R + ")*" + "(" + U + ")*";
                    }
                    else if((U[0] != '(' or U[U.size()-1] != ')') or U.size() == 1){
                        str = "(" + R + ")*" + U + "*";
                    }
                    else{
                        str = "(" + R + ")*";
                    }
                }
                else{
                    if((U[0] != '(' or U[U.size()-1] != ')') and U.size() > 1){
                        str = R + "*(" + U + ")*";
                    }
                    else if(U.size() == 1){
                        str = R + "*" + U + "*";
                    }
                    else{
                        str = R + "*";
                    }
                }
            }
            else if(U.empty()){
                if((R[0] != '(' or R[R.size()-1] != ')') and R.size()>1){
                    str = "(" + R + ")*" + S;
                }
                else{
                    str = R + "*" + S;
                }
            }
            else{
                if((R[0] != '(' or R[R.size()-1] != ')') and R.size()>1){
                    if((U[0] != '(' or U[U.size()-1] != ')') and U.size() > 1){
                        str = "(" + R + ")*" + S + "(" + U + ")" + "*";
                    }
                    else{
                        str = "(" + R + ")*" + S + U + "*";
                    }
                }
                else{
                    if((U[0] != '(' or U[U.size()-1] != ')') and U.size() > 1){
                        str = R + "*" + S + "(" + U + ")*";
                    }
                    else{
                        str = R + "*" + S + U + "*";
                    }
                }
            }
        }
        else{
            if((U[0] != '(' or U[U.size()-1] != ')') and U.size() > 1){
                str = "(" + R + "+" + S + "(" + U + ")*" + T + ")*" + S + "(" + U + ")*";
            }
            else{
                str = "(" + R + "+" + S + U + "*" + T + ")*" + S + U + "*";
            }
        }
    }
    return str;
}

DFA DFA::minimize() {
    vector<pair<string,string>> equivalent;
    vector<string> nietEquivalent;
    vector<string> states={StartingState};
    vector<string> finals=FinalStates;
    CurrentState=StartingState;
    for(const auto& transition:transitions[CurrentState]){
        if(std::find(states.begin(), states.end(),transition.at(1))==states.end()) {
            states.push_back(transition.at(1));
        }
    }
    for(const auto& state:states){
        for(const auto& state2:states){
            if(state==state2){
                continue;
            }
            else {
                //Todo: maak hulpfunctie
                bool distinguishable = false;
                for (int i = 0; i < alphabet.size(); i++) {
                    string toFind = transitions[state][i][0];
                    if (std::find(FinalStates.begin(), FinalStates.end(), transitions[state][i][0]) !=
                        FinalStates.end()) {
                        if (std::find(FinalStates.begin(), FinalStates.end(), transitions[state2][i][0]) !=
                            FinalStates.end()) {
                            continue;
                        } else {
                            distinguishable = true;
                        }
                    } else {
                        if (std::find(FinalStates.begin(), FinalStates.end(), transitions[state2][i][0]) !=
                            FinalStates.end()) {
                            distinguishable = true;
                        }
                    }
                }
                if (not distinguishable) {
                    equivalent.push_back({state, state2});
                }
            }
        }
    }
    //Todo: maak hulpfunctie
    vector<vector<string>> comboStates={};
    string nieuwStart;
    for(const auto& duo:equivalent){
        for(const auto& duo2:equivalent){
            if(duo==duo2){
                continue;
            }
            if(duo.first==duo2.first){
                comboStates.push_back({duo.first, duo.second, duo2.second});
            }
            else if(duo.first==duo2.second){
                comboStates.push_back({duo.first, duo.second, duo2.first});
            }
            else if(duo.second==duo2.first){
                comboStates.push_back({duo.first, duo.second, duo2.second});
            }
            else if(duo.second==duo2.second){
                comboStates.push_back({duo.first, duo.second, duo2.first});
            }
            else{
                comboStates.push_back({duo.first,duo.second});
                comboStates.push_back({duo2.first,duo2.second});
            }
        }
    }
    vector<string> nieuwStates={};
    for(auto state:states){
        for(auto same:equivalent){
            if(state!=same.first and state!=same.second){
                if(std::find(nietEquivalent.begin(), nietEquivalent.end(), state) == nietEquivalent.end())
                    nietEquivalent.push_back(state);
            }
        }
    }
    bool starting=false;
    for(auto collection:comboStates){
        string nieuwState;
        for(auto state:collection){
            nieuwState+=state+"/";
            if(state==StartingState){
                starting=true;
            }
        }
        if(std::find(nieuwStates.begin(), nieuwStates.end(), nieuwState) == nieuwStates.end())
            nieuwStates.push_back(nieuwState);
        if(starting) {
            nieuwStart=nieuwState;
        }
    }
    DFA nieuwDfa=DFA();
    nieuwDfa.alphabet=alphabet;
    bool final=false;
    if(std::find(nietEquivalent.begin(), nietEquivalent.end(),StartingState)!=nietEquivalent.end()){
        nieuwDfa.StartingState=StartingState;
    }
    else {
        nieuwDfa.StartingState=nieuwStart;
    }
    nieuwDfa.CurrentState=StartingState;
    vector<string> totalStates;
    nietEquivalent.insert(nietEquivalent.end(),nieuwStates.begin(),nieuwStates.end());
    totalStates=nietEquivalent;
    string transition;
    for(const auto& state:totalStates){
        if(state.find("/")!=-1){
            int pos=state.find("/");
            string copyState=state;
            copyState.substr(pos);
            if(std::find(FinalStates.begin(), FinalStates.end(),copyState)!=FinalStates.end()){
                final=true;
            }
            for(auto overgang:transitions[CurrentState]) {
                for (auto combo: comboStates) {
                    if(std::find(combo.begin(), combo.end(),overgang[1])!=combo.end()){
                        transition=overgang[0];
                    }
                }
            }
        }
        else if(std::find(FinalStates.begin(), FinalStates.end(),state)!=FinalStates.end()){
            final=true;
        }

        else{
            for(auto overgang:transitions[CurrentState]){
                if(overgang[1]==state){
                    transition=overgang[0];
                }
            }
        }
        nieuwDfa.addState(CurrentState,state,transition,final);
    }
    /*for(auto state:nieuwStates){

    }*/
    return nieuwDfa;
}

void DFA::addState(string from, string to, string transition, bool final) {
    if(std::find(alphabet.begin(), alphabet.end(), transition) == alphabet.end()){
        alphabet.push_back(transition);
    }
    if(final and find(FinalStates.begin(), FinalStates.end(), to) == FinalStates.end()){
        FinalStates.push_back(to);
    }
    if(transitions[from].empty()){
        transitions[from].push_back({transition, to});
    }
    else{
        bool b = true;
        for(auto i:transitions[from]){
            if(i[0] == transition){
                b = false;
                break;
            }
        }
        if(b){
            transitions[from].push_back({transition, to});
        }
    }
}

NFA DFA::toNFA() {
    NFA NFAboek;
    NFAboek.setStartingState(StartingState);
    NFAboek.setFinalStates(FinalStates);
    std::map<std::string,std::vector<std::vector<std::string>>> newTransitions;
    for(auto i:transitions){
        for(auto j:i.second){
            if(j[1] != "{}"){
               newTransitions[i.first].push_back({j[0], j[1]});
            }
        }
    }
    NFAboek.setTransitions(newTransitions);

    return NFAboek;
}