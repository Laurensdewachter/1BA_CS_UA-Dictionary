#ifndef DICTIONARY_SHELL_H
#define DICTIONARY_SHELL_H

#include "Woordenboek.h"
#include <iostream>
#include <exception>
#include <string>
#include <vector>

class Shell {
    bool running;
    bool windows;
    std::string prompt;
    Woordenboek* dictionary;

public:
    /**
     * \brief Constructor for the interactive dictionary shell
     * \author Laurens De Wachter
     */
    Shell();

    /**
     * \brief Destructor for the interactive dictionary shell
     * \author Laurens De Wachter
     */
    ~Shell() = default;

    /**
     * \brief Launches the interactive dictionary shell
     * \author Laurens De Wachter
     */
    void launch();

private:
    void run_line(std::string &cmd);

    void run_exit();
    void run_help();
    void run_save(std::vector<std::string> &args);
    void run_load(std::vector<std::string> &args);
    void run_add(std::vector<std::string> &args);
};

bool FileExists(const std::string &filename);
bool containsOnlyLetters(std::string const &str);

#endif