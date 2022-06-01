//
// Created by sisoo on 3/04/2022.
//

#include "RE.h"

RE::RE(const std::string &str, char epsilon) : str(str), epsilon(epsilon) {}

void RE::print() {
    std::cout << str << std::endl;
}

RE::RE() {}

const std::string &RE::getStr() const {
    return str;
}

void RE::setStr(const std::string &str) {
    RE::str = str;
}
