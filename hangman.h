#ifndef HANGMAN_H
#define HANGMAN_H

#include "gba.h"
#include "main.h"

void wordToOutput(char *input, char *output);
int enterLetter(char *word, char *blanks, char letter);
char letterSelected(int x, int y, int state);
struct position moveBox(int x, int y, u32 currentButtons, u32 previousButtons, int numIncorrect, struct position *wrongGuessPositions);
struct position moveBoxWord(int x, int y, u32 currentButtons, u32 previousButtons);
int drawBodyPart(int numLimbs);
struct position incorrectGuess(int x, int y, int numIncorrect, struct position *wrongGuessPositions);
int checkIfGuessed(int x, int y, int numIncorrect, struct position *wrongGuessPositions);

#endif
