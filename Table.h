//
// Created by robbe on 1-4-2022.
//

#ifndef TA_TABLE_H
#define TA_TABLE_H
#include <vector>
#include "json.hpp"
class DFA;
class State;
class Table {
public:
    std::vector<std::pair<std::string,std::vector<std::pair<bool,std::string>>>> table;
    std::vector<std::string> botemIndex;
public:
    Table();
    Table(DFA& dfa);

    void mark(State* x, State* y);
    void mark(std::string x, std::string y);

    bool checkSlot(DFA& dfa, State* x, State* y);
    bool checkSlot(DFA& dfa, std::string x, std::string y);

    void print();
    DFA minimize(DFA& dfa);

    bool getPosition(std::string x, std::string y);

private:
    void mergeStates(std::vector<std::vector<State*>>& vector,std::string x,std::string y);
};


#endif //TA_TABLE_H
