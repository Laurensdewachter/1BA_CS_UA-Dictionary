//
// Created by robbe on 1-4-2022.
//

#include "Table.h"
#include "DFA.h"
#include "State.h"
#include <iostream>

Table::Table() {
    table = {};
}

Table::Table(DFA& dfa) {
    // empty table
    std::vector<std::pair<bool, std::string>> temp = {};
    std::vector<State *> statesNS = dfa.getStates();
    std::vector<State *> states = {};
    std::vector<std::string> stateNames;
    for (State *s: statesNS) {
        stateNames.emplace_back(s->getName());
    }
    std::sort(stateNames.begin(), stateNames.end());
    for (auto i: stateNames) {
        states.emplace_back(dfa.getState(i));
    }
    for (int i = 0; i < states.size() - 1; i++) {
        temp.insert(temp.begin(), std::pair<bool, std::string>(false, "-"));
        table.emplace_back(
                std::pair<std::string, std::vector<std::pair<bool, std::string>>>(states[i + 1]->getName(), temp));
        botemIndex.emplace_back(states[i]->getName());
    }
    std::sort(botemIndex.begin(), botemIndex.end());
    // fill final states
    for (State *stateF: states) {
        if (stateF->isFinal()) {
            for (State *s: states) {
                if (!s->isFinal()) {
                    mark(stateF, s);
                }
            }
        }
    }
    // check grid
    bool update = true;
    while (update) {
        update = false;
        for (int row = 0; row < table.size(); row++) {
            for (int col = 0; col < table[row].second.size(); col++) {
                std::pair<bool, std::string> *position = &table[row].second[col];
                if (!position->first and position->second == "X") {
                    if (checkSlot(dfa,table[row].first,botemIndex[col])){
                        update = true;
                    }
                    position->first = true;
                }
            }
        }
    }
}

















void Table::mark(State* x, State* y) {
    mark(x->getName(),y->getName());
}

void Table::mark(std::string x, std::string y) {
    for (int _ = 0;_<2;_++) {
        for (int row = 0; row < table.size(); row++) {
            if (table[row].first == x) {
                for (int col = 0; col < botemIndex.size(); col++) {
                    if (botemIndex[col] == y) {
                        if (col <= row) {
                            table[row].second[col].second = "X";
                            return;
                        }
                        break;
                    }
                }
            }
        }
        std::swap(x,y);
    }
}

void Table::print() {
    for (auto row : table){
        std::cout<<row.first<<"  ";
        for (auto i : row.second){
            std::cout<<i.second<<"  ";
        }
        std::cout<<std::endl;
    }
    std::cout<<" ";
    for (auto col : botemIndex){
        std::cout<<"  "<<col;
    }
    std::cout<<std::endl;
}

bool Table::checkSlot(DFA &dfa, std::string x, std::string y) {
    bool update = false;
    std::vector<State*> states = dfa.getStates();
    std::vector<std::pair<nlohmann::json,std::string>> trans;
    for (State* state : states){
        for (auto input : dfa.getAlphabet()) {
            State* transTo = dfa.transition(state,input);
            if (transTo->getName() == x){
                for (State* state2 : states){
                    State* transTo2 = dfa.transition(state2,input);
                    if (transTo2->getName() == y) {
                        mark(state, state2);
                        update = true;
                    }
                }
            }
        }
    }
    return update;
}

bool Table::checkSlot(DFA &dfa, State *x, State *y) {
    return checkSlot(dfa,x->getName(),y->getName());
}

void Table::mergeStates(std::vector<std::vector<State *>> &vector, std::string x, std::string y) {
    for (auto VstateX = vector.begin(); VstateX != vector.end(); VstateX++) {
        for (State *state: *VstateX) {
            if (state->getName() == x) {
                for (State *statecheck: *VstateX){
                    if (statecheck->getName() == y){
                        return;
                    }
                }
                for (auto VstateY = vector.begin(); VstateY != vector.end(); VstateY++) {
                    for (State *state2: *VstateY) {
                        if (state2->getName() == y) {
                            VstateX->insert(VstateX->begin(), VstateY->begin(), VstateY->end());
                            vector.erase(VstateY);
                            return;
                        }
                    }
                }
            }
        }
    }
}


DFA Table::minimize(DFA& dfa) {
    std::vector<std::vector<State *>> minStates = {};
    for (State *state: dfa.getStates()) {
        minStates.emplace_back(std::vector<State *>({state}));
    }
    for (int row = 0; row < table.size(); row++) {
        for (int col = 0; col < table[row].second.size(); col++) {
            if (table[row].second[col].second == "-") {
                mergeStates(minStates, table[row].first, botemIndex[col]);
            }
        }
    }
    DFA mindfa = DFA(dfa.getAlphabet());
    bool start = false;
    for (auto states: minStates) {
        State *newstate = new State(dfa.getName(states), states[0]->isFinal());
        if (!start) {
            for (State *state: states) {
                if (state == dfa.getStart()) {
                    start = true;
                }
            }
            mindfa.addState(newstate, start);
        } else {
            mindfa.addState(newstate);
        }
    }
    State *tr;
    bool add;
    bool add2;
    for (auto Vstates: minStates) {
        State *minState = mindfa.getState(dfa.getName(Vstates));

        for (auto input: mindfa.getAlphabet()) {
            add2 = true;
            for (auto state: Vstates) {
                add = add2;
                tr = dfa.transition(state, input);
                for (auto statecheck: Vstates) {
                    if (tr == statecheck) {
                        add = false;
                    }
                }
                if (add) {
                    for (std::vector<State *> Vs: minStates) {
                        for (State *s: Vs) {
                            if (s == tr) {
                                minState->addTransition(input,dfa.getName(Vs));
                                add2 = false;
                                break;
                            }
                        }
                    }
                }
            }
            if (add2) {
                minState->addTransition(input, minState->getName());
            }
        }
    }
    return mindfa;
}

bool Table::getPosition(std::string x, std::string y) {
    for (int _ = 0; _ < 2; _++) {
        for (int row = 0; row < table.size(); row++) {
            if (table[row].first == x) {
                for (int col = 0; col < botemIndex.size(); col++) {
                    if (botemIndex[col] == y) {
                        if (col <= row) {
                            return (table[row].second[col].second == "X");
                        }
                        break;
                    }
                }
            }
        }
        std::swap(x, y);
    }
    return false;
}