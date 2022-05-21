#include "Shell.h"
#include <vector>
#include <algorithm>

Shell::Shell(bool windows) : running(false), windows(windows), prompt("> ") {}

void Shell::launch() {
    running = true;
    std::cout << "Welcome to the interactive dictionary.\nUse \"help\" or \"h\" to get a list of all accepted commands." << std::endl;
    while (running) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        run_line(line);
    }
}

void Shell::run_line(std::string &cmd) {
    // remove whitespaces in front
    for (long unsigned int i = 0; i < cmd.length(); i++) {
        if (cmd[i] == ' ') cmd = cmd.substr(1, cmd.length()-1);
        else break;
    }
    // remove comments
    for (long unsigned int i = 0; i < cmd.length(); i++) {
        if (cmd[i] == '#') cmd = cmd.substr(0, i);
        if (cmd.empty()) return;
    }
    // remove whitespace at end of line
    if (cmd[cmd.length()-1] == ' ') cmd.pop_back();
    // check if the string contains characters
    bool allWhite = true;
    for (auto i : cmd) if (i != ' ') allWhite = false;
    if (allWhite) return;
    // split command from arguments
    std::string curWord;
    bool commandFound = false;
    std::string command;
    std::vector<std::string> args;
    for (auto i : cmd) {
        if (i == ' ') {
            if (!commandFound) {
                command = curWord;
                commandFound = true;
            } else {
                args.push_back(curWord);
                curWord.clear();
            }
        } else curWord += i;
    }
    if (!commandFound) command = curWord;
    else args.push_back(curWord);
    // make the command lowercase
    std::for_each(command.begin(), command.end(), [](char &c){c = std::tolower(c);});

    try {
        if (command == "exit") run_exit();
        else if (command == "help" || command == "h") run_help();
        else if (command == "save") run_save(args);
        else if (command == "load") run_load(args);
        else {
            throw std::invalid_argument("\"" + command + "\" is an unknown command.\nUse \"help\" or \"h\" to get a list of all accepted commands.");
        }
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}

void Shell::run_exit() {
    std::cout << "Are you sure you want to leave the dictionary shell? [y/n] ";
    std::string answer;
    std::getline(std::cin, answer);
    std::transform(answer.begin(), answer.end(), answer.begin(),
                   [](unsigned char c){ return std::tolower(c); });
checkAnswer:
    if (answer == "y" || answer == "yes") running = false;
    else if (answer != "n" && answer != "no") {
        std::cout << "Are you sure you want to leave the dictionary shell? [y/n] ";
        std::getline(std::cin, answer);
        std::transform(answer.begin(), answer.end(), answer.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        goto checkAnswer;
    }
}

void Shell::run_help() {
    std::cout << "Commands:" << std::endl
    << R"("exit": )" << "\t\t\t\t\t\t\t" << "quit the interactive dictionary" << std::endl
    << R"("help" or "h": )" << "\t\t\t\t\t\t" << "display this screen" << std::endl
    << R"("save [DIR]": )" << "\t\t\t\t\t\t" << "save the current dictionary in the given (optional) directory." << std::endl
    << R"("load [FILE]": )" << "\t\t\t\t\t\t" << "load a dictionary from the given file" << std::endl;
}

void Shell::run_save(std::vector<std::string> &args) {
    std::string dir;
    if (args.empty()) dir = "";
    else dir = args[0] + "/";

    std::string fileName = dir + "dictionary.json";
}

void Shell::run_load(std::vector<std::string> &args) {
    if (args.empty()) throw std::invalid_argument("\"load\" should given a file name as an argument.\nUse \"help\" or \"h\" to get a list of all accepted commands.");
}