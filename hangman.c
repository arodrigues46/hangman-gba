
#include "hangman.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "images/hangman_home.h"
#include "images/hangman_word.h"
#include "images/hangman_play.h"
#include "images/hangman_head.h"
#include "images/hangman_body.h"
#include "images/hangman_leg1.h"
#include "images/hangman_leg2.h"
#include "images/hangman_arm1.h"
#include "images/hangman_arm2.h"
#include "images/hangman_win.h"
#include "images/hangman_lose.h"


#include "main.h"
#include <string.h>

void wordToOutput(char input[], char output[]) {
    int j = 0;
    for (size_t i = 0; i < strlen(input); i++) {
        if (input[i] == ' ') {
            output[j] = ' ';
        } else {
            output[j] = '_';
        }
        output[j + 1] = ' ';
        j = j + 2;
    }
    output[j - 1] = '\0';
}

int enterLetter(char word[], char blanks[], char letter) {
    int inWord = 0;
    int numBlanks = 0;

    for (size_t i = 0; i < strlen(word); i++) {
        if (word[i] == letter) {
            blanks[i * 2] = letter;
            inWord = 1;
        }
        if (blanks[i * 2] == '_') {
            numBlanks++;
        }
    }

    if (numBlanks == 0) {
        return 3;
    }

    return inWord;
}

char letterSelected(int x, int y, int state) {
    int numLetter;
    int xPos;
    if (state == 0) {
        xPos = 108;
    } else {
        xPos = 111;
    }
    if (x == xPos) {
        numLetter = (y - 8) / 18;
    } else {
        numLetter = ((y - 8) / 18) + 13;
    }
    return numLetter + 'A';
}

struct position moveBox(int x, int y, u32 currentButtons, u32 previousButtons, int numIncorrect, struct position *wrongGuessPositions) {
    undrawImageDMA(x, y, 14, 14, hangman_play);
    struct position newPosition;
    newPosition.x = x;
    newPosition.y = y;
    if (KEY_JUST_PRESSED(BUTTON_RIGHT, currentButtons, previousButtons)) {
        if (y == 224) {
            newPosition.x = x;
            newPosition.y = 8;
            while (checkIfGuessed(newPosition.x, newPosition.y, numIncorrect, wrongGuessPositions) == 1) {
                newPosition.y += 18;
            }
        } else {
            newPosition.x = x;
            newPosition.y = y + 18;
            while (checkIfGuessed(newPosition.x, newPosition.y, numIncorrect, wrongGuessPositions) == 1) {
                if (newPosition.y == 224) {
                    newPosition.y = 8;
                } else {
                    newPosition.y += 18;
                }
            }
        }
    } else if (KEY_JUST_PRESSED(BUTTON_LEFT, currentButtons, previousButtons)) {
        if (y == 8) {
            newPosition.x = x;
            newPosition.y = 224;
            while (checkIfGuessed(newPosition.x, newPosition.y, numIncorrect, wrongGuessPositions) == 1) {
                newPosition.y -= 18;
            }
        } else {
            newPosition.x = x;
            newPosition.y = y - 18;
            while (checkIfGuessed(newPosition.x, newPosition.y, numIncorrect, wrongGuessPositions) == 1) {
                if (newPosition.y == 8) {
                    newPosition.y = 224;
                } else {
                    newPosition.y -= 18;
                }
            }
        }
    } else if (KEY_JUST_PRESSED(BUTTON_UP, currentButtons, previousButtons)) {
        if (x == 111) {
            if (checkIfGuessed(127, y, numIncorrect, wrongGuessPositions) == 0) {
                newPosition.x = 127;
                newPosition.y = y;
            }
        } else {
            if (checkIfGuessed(111, y, numIncorrect, wrongGuessPositions) == 0) {
                newPosition.x = 111;
                newPosition.y = y;
            }
        }
    } else if (KEY_JUST_PRESSED(BUTTON_DOWN, currentButtons, previousButtons)) {
        if (x == 111) {
            if (checkIfGuessed(127, y, numIncorrect, wrongGuessPositions) == 0) {
                newPosition.x = 127;
                newPosition.y = y;
            }
        } else {
            if (checkIfGuessed(111, y, numIncorrect, wrongGuessPositions) == 0) {
                newPosition.x = 111;
                newPosition.y = y;
            }
        }
    }
    drawRectDMA(newPosition.x, newPosition.y, 14, 14, MAGENTA);
    undrawImageDMA(newPosition.x + 1, newPosition.y + 1, 12, 12, hangman_play);

    return newPosition;
}

struct position moveBoxWord(int x, int y, u32 currentButtons, u32 previousButtons) {
    undrawImageDMA(x, y, 14, 14, hangman_word);
    struct position newPosition;
    newPosition.x = x;
    newPosition.y = y;
    if (KEY_JUST_PRESSED(BUTTON_RIGHT, currentButtons, previousButtons)) {
        if (y == 224) {
            newPosition.x = x;
            newPosition.y = 8;
        } else {
            newPosition.x = x;
            newPosition.y = y + 18;
        }
    } else if (KEY_JUST_PRESSED(BUTTON_LEFT, currentButtons, previousButtons)) {
        if (y == 8) {
            newPosition.x = x;
            newPosition.y = 224;
        } else {
            newPosition.x = x;
            newPosition.y = y - 18;
        }
    } else if (KEY_JUST_PRESSED(BUTTON_UP, currentButtons, previousButtons)) {
        if (x == 108) {
            newPosition.x = 123;
            newPosition.y = y;
        } else {
            newPosition.x = 108;
            newPosition.y = y;
        }
    } else if (KEY_JUST_PRESSED(BUTTON_DOWN, currentButtons, previousButtons)) {
        if (x == 108) {
            newPosition.x = 123;
            newPosition.y = y;
        } else {
            newPosition.x = 108;
            newPosition.y = y;
        }
    }
    drawRectDMA(newPosition.x, newPosition.y, 14, 14, MAGENTA);
    undrawImageDMA(newPosition.x + 1, newPosition.y + 1, 12, 12, hangman_word);

    return newPosition;
}

int drawBodyPart(int numLimbs) {
    if (numLimbs == 1) {
        drawImageDMA(5, 75, 80, 80, hangman_head);
    } else if (numLimbs == 2) {
        drawImageDMA(5, 75, 80, 80, hangman_body);
    } else if (numLimbs == 3) {
        drawImageDMA(5, 75, 80, 80, hangman_leg1);
    } else if (numLimbs == 4) {
        drawImageDMA(5, 75, 80, 80, hangman_leg2);
    } else if (numLimbs == 5) {
        drawImageDMA(5, 75, 80, 80, hangman_arm1);
    } else if (numLimbs == 6) {
        drawImageDMA(5, 75, 80, 80, hangman_arm2);
        return 1;
    }
    return 0;
}

struct position incorrectGuess(int x, int y, int numIncorrect, struct position *wrongGuessPositions) {
    drawRectDMA(x, y, 14, 14, GRAY);
    struct position newPosition;
    if (y == 224) {
        newPosition.x = x;
        newPosition.y = 8;
        while (checkIfGuessed(newPosition.x, newPosition.y, numIncorrect, wrongGuessPositions) == 1) {
            newPosition.y += 18;
        }
    } else {
        newPosition.x = x;
        newPosition.y = y + 18;
        while (checkIfGuessed(newPosition.x, newPosition.y, numIncorrect, wrongGuessPositions) == 1) {
            newPosition.y = newPosition.y + 18;
        }
    }
    drawRectDMA(newPosition.x, newPosition.y, 14, 14, MAGENTA);
    undrawImageDMA(newPosition.x + 1, newPosition.y + 1, 12, 12, hangman_play);

    return newPosition;
}

int checkIfGuessed(int x, int y, int numIncorrect, struct position *wrongGuessPositions) {
    for (int i = 0; i < numIncorrect; i++) {
        if (wrongGuessPositions[i].x == 0 && wrongGuessPositions[i].y == 0) {
            return 0;
        }
        if (wrongGuessPositions[i].x == x && wrongGuessPositions[i].y == y) {
            return 1;
        }
    }

    return 0;
}