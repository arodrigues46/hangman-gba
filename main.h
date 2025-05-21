#ifndef MAIN_H
#define MAIN_H

#include "gba.h"
#include "hangman.h"

// TODO: Create any necessary structs

struct position {
    int x;
    int y;
};

struct word {
    char word[15];
    char blanks[30];
    int wordPos;
    int numLetters;
};

struct wrongGuesses {
    struct position wrongGuessPositions[6];
    int numIncorrect;
    int wrongLetter;
};

struct boxLetter {
    struct position boxPosition;
    char letter;
    int letterChosen;
    int shiftBox;
};

#endif
