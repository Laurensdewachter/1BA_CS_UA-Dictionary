#ifndef DICTIONARY_SHELL_H
#define DICTIONARY_SHELL_H

#include <iostream>
#include <exception>
#include <string>
#include <vector>

class Shell {
    bool running;
    bool windows;
    std::string prompt;
public:
    Shell(bool windows);
    ~Shell() = default;

    /**
     * \brief Launches the interactive dictionary shell
     */
    void launch();

private:
    void run_line(std::string &cmd);

    void run_exit();
    void run_help();
    void run_save(std::vector<std::string> &args);
    void run_load(std::vector<std::string> &args);
};


#endif