#include "Shell.h"
#include "Wordle.h"
#include <vector>
#include <algorithm>
#include <fstream>
#include <sys/stat.h>

Shell::Shell() : running(false), prompt("> ") {
    dictionary = new Woordenboek;
}

void Shell::launch() {
    running = true;
    std::cout << "Welcome to the interactive dictionary." << std::endl
    << R"(Use "help" or "h" to get a list of all accepted commands.)" << std::endl;
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
                curWord.clear();
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
        else if (command == "add") run_add(args);
        else if (command == "remove") run_remove(args);
        else if (command == "minimize") run_minimize(args);
        else if (command == "check") run_check_text(args);
        else if (command == "combine") run_combine(args);
        else if (command == "length") run_length(args);
        else if (command == "suggest") run_suggest(args);
        else if (command == "wordle") run_wordle(args);
        else if (command == "equal") run_equal(args);
        else if (command == "save") run_save(args);
        else if (command == "load") run_load(args);
        else if (command == "clear") system("clear");
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
    if (answer == "y" || answer == "yes") {
        delete dictionary;
        running = false;
    }
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
    << R"("exit":)" << "\t\t\t\t\t\t\t" << "quit the interactive dictionary" << std::endl
    << R"("help" or "h":)" << "\t\t\t\t\t\t" << "display this screen" << std::endl
    << R"("add [WORD]":)" << "\t\t\t\t\t\t" << "add a word to the dictionary" << std::endl
    << R"("remove [WORD]":)" << "\t\t\t\t\t" << "remove a word from the dictionary" << std::endl
    << R"("minimize":)" << "\t\t\t\t\t\t" << "minimize the current dictionary" << std::endl
    << R"("check [FILE]":)" << "\t\t\t\t\t\t" << "check the text in the file for mistakes" << std::endl
    << R"("combine [FILE]":)" << "\t\t\t\t\t" << "combine the current dictionary with the dictionary in the file" << std::endl
    << R"("length [N]":)" << "\t\t\t\t\t\t" << "remove all words that do not have length N from the dictionary" << std::endl
    << R"("suggest [WORD]":)" << "\t\t\t\t\t" << "suggest a word in the dictionary that is the most similar to WORD" << std::endl
    << R"("wordle":)" << "\t\t\t\t\t\t" << "play a game of Wordle" << std::endl
    << R"("equal [FILE]":)" << "\t\t\t\t\t\t" << "compare the current dictionary in the file" << std::endl
    << R"("save":)" << "\t\t\t\t\t\t\t" << "save the current dictionary" << std::endl
    << R"("load [FILE]":)" << "\t\t\t\t\t\t" << "load a dictionary from the given file" << std::endl;
}

void Shell::run_add(std::vector<std::string> &args) {
    if (args.empty()) throw std::invalid_argument("\"add\" should be given a word as an argument.\nUse \"help\" or \"h\" to get a list of all accepted commands");
    if (!containsOnlyLetters(args[0])) throw std::invalid_argument("Word \"" + args[0] + "\" is not valid.\nThe word can only contain letters");
    std::string word = args[0];
    for (unsigned int i = 1; i < args.size(); i++) {
        word += " " + args[i];
    }
    std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c){ return std::tolower(c);});

    dictionary->addWoord(word);
}

void Shell::run_remove(std::vector<std::string> &args) {
    if (args.empty()) throw std::invalid_argument("\"remove\" should be given a word as an argument.\nUse \"help\" or \"h\" to get a list of all accepted commands");
    if (!containsOnlyLetters(args[0])) throw std::invalid_argument("Word \"" + args[0] + "\" is not valid.\nThe word can only contain letters");
    std::string word = args[0];
    for (unsigned int i = 1; i < args.size(); i++) {
        word += " " + args[i];
    }
    std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c){ return std::tolower(c);});

    dictionary->removeWoord(word);
}

void Shell::run_minimize(std::vector<std::string> &args) {
    if (!args.empty()) throw std::invalid_argument("\"minimize\" should not be given an argument.\nUse \"help\" or \"h\" to get a list of all accepted commands");

    dictionary->minimaliseer();
}

void Shell::run_check_text(std::vector<std::string> &args) {
    if (args.size() != 1) throw std::invalid_argument("\"check\" should be given a file as an argument.\nUse \"help\" or \"h\" to get a list of all accepted commands");
    if (!FileExists(args[0])) throw std::invalid_argument("\"" + args[0] + "\" does not exist. Please give a valid file");

    dictionary->controleer(args[0]);
}

void Shell::run_combine(std::vector<std::string> &args) {
    if (args.size() != 1) throw std::invalid_argument("\"combine\" should be given a file as an argument.\nUse \"help\" or \"h\" to get a list of all accepted commands");
    std::string fileName1 = args[0];
    if (!FileExists(fileName1)) {
        throw std::invalid_argument("\"" + fileName1 + "\" does not exist. Please give a valid file");
    }

    Woordenboek dict2(args[0]);

    dictionary->combineer(dict2);
}

void Shell::run_length(std::vector<std::string> &args) {
    if (args.size() != 1) throw std::invalid_argument("\"length\" should be given a number as an argument.\nUse \"help\" or \"h\" to get a list of all accepted commands");
    if (!isNumber(args[0])) throw std::invalid_argument("\"" + args[0] + "\" is not a number");

    dictionary->getWoordenboekVanLengte(std::stoi(args[0]));
}

void Shell::run_suggest(std::vector<std::string> &args) {
    if (args.empty()) {
        args.emplace_back("");
    } else {
        if (!containsOnlyLetters(args[0])) throw std::invalid_argument("Word \"" + args[0] + "\" is not valid.\nThe word can only contain letters");
    }
    std::string word = args[0];
    for (unsigned int i = 1; i < args.size(); i++) {
        word += " " + args[i];
    }

    std::string suggestion = dictionary->giveSuggestion(word);

    std::cout << "The closest word in the dictionary is \"" << suggestion << "\"" << std::endl;
}

void Shell::run_wordle(std::vector<std::string> &args) {
    if (!args.empty()) throw std::invalid_argument("\"wordle\" should not be given an argument.\nUse \"help\" or \"h\" to get a list of all accepted commands");

    Wordle wordle(dictionary);
    prompt = "guess > ";
    while(!wordle.hasFinished()) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        std::transform(line.begin(), line.end(), line.begin(), [](unsigned char c){ return std::tolower(c);});
        if (line == "exit wordle") break;
        if (!containsOnlyLetters(line)) continue;

        wordle.guessWord(line);
    }
    prompt = "> ";
}

void Shell::run_equal(std::vector<std::string> &args) {
    if (args.size() != 1) throw std::invalid_argument("\"equal\" should be given a file as an argument.\nUse \"help\" or \"h\" to get a list of all accepted commands");
    if (!FileExists(args[0])) throw std::invalid_argument("\"" + args[0] + "\" does not exist. Please give a valid file");

    Woordenboek dict2(args[0]);

    dictionary->gemeenschappelijk(dict2);
}

void Shell::run_save(std::vector<std::string> &args) {
    if (!args.empty()) throw std::invalid_argument("\"save\" should not be given an argument.\nUse \"help\" or \"h\" to get a list of all accepted commands");

    std::string fileName = "dictionary.txt";

    std::ofstream stream;
    stream.open(fileName);
    dictionary->save(stream);
    stream.close();

    std::cout << R"(The dictionary was saved as ")" + fileName + R"(".)" << std::endl;
}

void Shell::run_load(std::vector<std::string> &args) {
    if (args.size() != 1) throw std::invalid_argument("\"load\" should be given a file name as an argument.\nUse \"help\" or \"h\" to get a list of all accepted commands");
    if (!FileExists(args[0])) throw std::invalid_argument("File \"" + args[0] + "\" does not exist");

    delete dictionary;
    dictionary = new Woordenboek(args[0]);
}

bool FileExists(const std::string &filename) {
    struct stat st;
    if (stat(filename.c_str(), &st) != 0) return false;
    std::ifstream f(filename.c_str());
    if (f.good()) {
        f.close();
        return true;
    } else {
        f.close();
        return false;
    }
}

bool containsOnlyLetters(std::string const &str) {
    return str.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") == std::string::npos;
}

bool isNumber(const string &str) {
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}
