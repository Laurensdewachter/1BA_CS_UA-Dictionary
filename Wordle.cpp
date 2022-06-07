#include "Wordle.h"
#include <iostream>
#include <iomanip>
#include <random>

Wordle::Wordle(Woordenboek* book, unsigned int length, unsigned int numguesses) {
    numGuesses = numguesses;
    //w = book->getWoordenboekVanLengte(length);
    w = book;
    while (word.length() != length) word = book->woorden[rand() % (book->woorden.size())];
    wordLength = length;
    guesses = {};
    colors = {};
    print();
    std::cout << std::endl << std::endl;
}

void Wordle::print() {
    std::cout << "\033[2J" << "\033[0;0H";
    std::string baseColor = "\033[1;35m";
    std::cout << baseColor;

    std::cout<<"----";
    for (unsigned int i =0; i< wordLength; i++){
        std::cout<< "---";
    }

    std::cout<<std::endl;
    for (unsigned int i =0; i< numGuesses; i++) {
        if (i >= guesses.size()) {//empty line
            std::cout << "|" << std::setw(wordLength * 3 + 3) << "|" << std::endl;
        } else {
            std::cout << "|  ";
            for (unsigned int j = 0 ; j < guesses[i].size(); j++) {
                std::cout << colors[i][j] << guesses[i][j] << "  ";
            }
            std::cout << baseColor << "|"<< std::endl;
        }
    }

    std::cout<<"----";
    for (unsigned int i =0; i< wordLength; i++){
        std::cout<< "---";
    }
    std::cout << "\033[1;38m" << std::endl;
}

void Wordle::guessWord(std::string guess) {
    if (guess.find_first_not_of("abcdefghijklmnopqrstuvwxyz") != std::string::npos){
        std::cout << "\"" << guess << "\" is not a valid word" << std::endl;
        return;
    }
    if (w->accepts(guess)) {
        guesses.emplace_back(guess);
        colors.emplace_back(std::vector<std::string>(wordLength, "\033[1;37m"));
        std::string copy = word;
        for (unsigned int i = 0; i < wordLength; i++) {
            if (guess[i] == word[i]){
                colors[colors.size() - 1][i] = "\033[1;32m";
                copy[i] = '_';
            } else{
                for (unsigned int j = 0; j < wordLength; j++) {
                    if (guess[i] == copy[j] and guess[j] != copy[j]){
                        colors[colors.size() - 1][i] = "\033[1;31m";
                        copy[j] = '_';
                        break;
                    }
                }
            }
        }
        print();
    } else {
        print();
        std::cout << "\"" << guess << "\" is not a valid word" << std::endl;
    }
    if (guess == word) {
        std::cout << "Victory!!!";
        finished = true;
        clear();
    }

    if (guesses.size() == numGuesses) {
        std::cout << "The word was: " << word << std::endl;
        std::cout << "Better Luck next time!!!" << std::endl;
        clear();
    }
    std::cout << std::endl << std::endl;
}

void Wordle::clear() {
    guesses = {};
    colors = {};
}