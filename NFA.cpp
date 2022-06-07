//
// Created by dante on 25/05/22.
//

#include "NFA.h"
#include <iostream>

NFA::NFA() {}

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
    CurrentState = StartingState;
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

void NFA::print(std::ostream &onstream) {
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
    onstream << setw(4) << j << endl;
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
    for(auto i:alphabet){
        addState("{}", "{}", i, false);
    }

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
    type = "NFA";
    if (b) {
        for(auto i:nfa1.alphabet){
            if(find(nfa2.alphabet.begin(), nfa2.alphabet.end(), i) != nfa2.alphabet.end()){
                alphabet.push_back(i);
            }
        }
        StartingState = "Start";
        for(auto i:nfa1.FinalStates){
            if(find(nfa2.FinalStates.begin(), nfa2.FinalStates.end(), i) != nfa2.FinalStates.end()){
                FinalStates.push_back(i);
            }
        }
        for(auto i:nfa1.transitions){
            if(nfa2.transitions.find(i.first) != nfa2.transitions.end()){
                for(auto j:i.second){
                    for(auto k: nfa2.transitions[i.first]){
                        if(k[1] == j[1]){
                            addState(i.first, j[1], j[0], std::find(FinalStates.begin(), FinalStates.end(), j[1]) != FinalStates.end());
                    }
                    }
                }
            }
        }
    } else {
        alphabet = nfa1.alphabet;
        pushalf(nfa2.alphabet);
        StartingState = "Start";
        FinalStates = nfa1.FinalStates;
        pushFinal(nfa2.FinalStates);
        transitions = nfa1.transitions;
        for (auto i: nfa2.transitions) {
            for (auto j: i.second) {
                if (transitions.find(j[1]) == transitions.end()) {
                    addState(i.first, j[1], j[0],
                             std::find(FinalStates.begin(), FinalStates.end(), j[1]) != FinalStates.end());
                }
            }
        }
    }
}

const string &NFA::getType() const {
    return type;
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

void NFA::pushalf(vector<string> alf) {
    for (auto i:alf){
        auto result = std::find(alphabet.begin(), alphabet.end(), i);
        if(result == alphabet.end()) {
            alphabet.push_back(i);
        }
    }
}

void NFA::makeStochastic(vector<string>& woorden) {
    stochasticTransitions = transitions;
    for(auto i:transitions[StartingState]){
        string str = i[1];
        int teller = 0;
        vector<string> newWords;
        for(auto j:woorden){
            string substr = j.substr(0,1);
            if(substr == str){
                newWords.push_back(j);
                teller ++;
            }
        }
        for(auto &k: stochasticTransitions[StartingState]){
            if(k[1] == str){
                k.push_back(to_string(teller));
                k.push_back(to_string(woorden.size()));
                break;
            }
        }
        makeBranchStoch(str, newWords, 2);
    }
}

void NFA::addState(string from, string to, string transition, bool final) {
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

void NFA::makeBranchStoch(string letters, vector<string> woorden, int number) {

    for(auto i:transitions[letters]){
        vector<string> newWords = {};
        int teller = 0;
        string str = letters+i[1].substr(i[1].size()-1, 1);
        for(auto j:woorden){
            string substr = j.substr(0,number);
            if(substr == str){
                newWords.push_back(j);
                teller ++;
            }
        }
        for(auto& k: stochasticTransitions[letters]){
            if(k[1] == str){
                k.push_back(to_string(teller));
                k.push_back(to_string(woorden.size()));
                break;
            }
        }
        if(!transitions[str].empty()){
            makeBranchStoch(str, newWords, number+1);
        }
    }
}

string NFA::getSuggestion(string letters, bool b) {
    if(b) {
        if(letters.empty()){
            letters = "Start";
        }
        if (stochasticTransitions[letters].empty() and
            find(FinalStates.begin(), FinalStates.end(), letters) != FinalStates.end()) {
            return letters;
        } else if (stochasticTransitions[letters].empty()) {
            return "";
        }
    }
    if (stochasticTransitions[letters].empty()) return "";
    int number = stoi(stochasticTransitions[letters][0][2]);
    string trans = stochasticTransitions[letters][0][0];
    for(auto i:stochasticTransitions[letters]){
        if(stoi(i[2])>number){
            number = stoi(i[2]);
            trans = i[0];
        }
    }
    if(letters == "Start"){
        return getSuggestion(trans, false);
    }
    else if(find(FinalStates.begin(), FinalStates.end(), letters+trans) != FinalStates.end()){
        string str = letters+trans;
        return str;
    }
    else{
        return getSuggestion(letters+trans, false);
    }
}

void NFA::pushFinal(vector<string> final) {
    for (auto i:final){
        auto result = std::find(FinalStates.begin(), FinalStates.end(), i);
        if(result == FinalStates.end()) {
            FinalStates.push_back(i);
        }
    }
}

void NFA::removeUnreachable() {
    // http://www.cs.um.edu.mt/gordon.pace/Research/Software/Relic/Transformations/FSA/remove-unreachable.html
    vector<string> preR;
    vector<string> R = {StartingState};
    while (preR != R) {
        preR = R;
        vector<string> M;
        for (const auto &preState: preR) {     // Voor elke state
            for (const auto &nextTransition: transitions[preState]) {    // voor elke letter
                M.insert(M.end(), nextTransition.begin() + 1, nextTransition.end());
            }
        }
        sort(M.begin(), M.end());
        R.clear();
        set_union(preR.begin(), preR.end(), M.begin(), M.end(), back_inserter(R));
    }
    map<string,vector<vector<string>>> newTransition;
    for (auto &transition: transitions) {
        if (find(R.begin(), R.end(), transition.first) == R.end()) {
            continue;
        }
        for (int i = 0; i < transition.second.size(); ++i) {
            auto &transitionSymbol = transition.second[i];
            newTransition[transition.first].push_back({transitionSymbol[0]});
            sort(transitionSymbol.begin() + 1, transitionSymbol.end());
            set_intersection(transitionSymbol.begin() + 1, transitionSymbol.end(), R.begin(), R.end(),
                             back_inserter(newTransition[transition.first][i]));
        }
    }
    transitions = newTransition;

    vector<string> oldFinalStates = FinalStates;
    FinalStates.clear();
    sort(oldFinalStates.begin(), oldFinalStates.end());
    set_intersection(oldFinalStates.begin(), oldFinalStates.end(), R.begin(), R.end(), back_inserter(FinalStates));
}

void NFA::minimizeUnreachable(string state) {
    if (not transitions[state].empty())
        return;
    for (int i = state.size() - 1; i > 0; i--) {
        string presubWoord = state.substr(0, i+1);
        string subWoord = state.substr(0, i);
        if (transitions[subWoord].size() == 1 && transitions[subWoord][0].size() == 2 && find(FinalStates.begin(),
                                                                                              FinalStates.end(),
                                                                                              subWoord) == FinalStates.end()) {
            continue;
        }
        for (int j = 0; j < transitions[subWoord].size(); j++) {
            if (transitions[subWoord][j][0] == presubWoord.substr(i,1)) {
                if (transitions[subWoord][j].size() == 2)
                    transitions[subWoord].erase(transitions[subWoord].begin() + j);
                else
                    transitions[subWoord][j].erase(find(transitions[subWoord][j].begin(),
                                                       transitions[subWoord][j].end(),
                                                       presubWoord));
                return;
            }
        }
    }
    transitions.erase({state[0]});
    for (int j = 0; j < transitions["Start"].size(); ++j) {
        if (transitions["Start"][j][0] == state.substr(0,1)) {
            transitions["Start"].erase(transitions["Start"].begin() + j);
            break;
        }
    }
}
