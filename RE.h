//
// Created by sisoo on 3/04/2022.
//

#ifndef TALEN_EN_AUTOMATEN_RE_H
#define TALEN_EN_AUTOMATEN_RE_H

#include "string"
#include "iostream"

class RE {
std::string str;
char epsilon;

public:
    RE(const std::string &str, char epsilon);

    RE();

    const std::string &getStr() const;

    void setStr(const std::string &str);

    void print();
};


#endif //TALEN_EN_AUTOMATEN_RE_H
