#include <iostream>
#include <windows.h>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <thread>
using namespace std;

struct Card {
    int backNumber;
    char frontLetter;
    bool isActive;
};

vector<vector<vector<Card>>> disp(4, vector<vector<Card>>(4, vector<Card>(1))); // Got very convoluted but necessary to pull from struct properly
unsigned int currentScore = 0;

void timer(unsigned int secInput) {
    unsigned int timer = secInput;
    unsigned int digitOutput = log10(timer) + 1;
    while (timer >= 1) {
        cout << setw(digitOutput) << timer << flush; // Preventing format error when timer goes down a digit
        Sleep(1000); // = 1 second, iterated by timer
        cout << '\r';
        timer--;
    }
    cout << endl;
}

void addScore(unsigned int& currentScore, unsigned int& currentTime) {
    double timeBonus = (1.0 / static_cast<double>(currentTime));
    currentScore += timeBonus;
}

void inputGuess(vector<vector<vector<Card>>>& disp) { //FIXME: need to add in code that prints disp array with the user-entered numbers being replaced by the corresponding letter
    int guess1, guess2;
    bool validGuess1 = false, validGuess2 = false;

    while (validGuess1 == false) { // May be worth making into separate function when cleaning up, but not a priority
        cout << "Enter the first card's number: ";
        cin >> guess1;
        if (cin.fail()) {
            cout << "\nOnly refer to cards by their numbers! Each card is numbered from 1 to 16. Please try again." << endl;
            cin.clear();
            cin.ignore();
            continue;
        }
        else if (guess1 < 1 || guess1 > 16) {
            cout << "\nCards are only numbered from 1 to 16! Please refer to a card within that range and try again." << endl;
            cin.clear();
            cin.ignore();
            continue;
        }
        else {
            validGuess1 = true;
        }
    }

    while (validGuess2 == false) {
        cout << "Enter the second card's number: ";
        cin >> guess2;
        if (cin.fail()) {
            cout << "\nOnly refer to cards by their numbers! Each card is numbered from 1 to 16. Please try again." << endl;
            continue;
        }
        else if (guess2 < 1 || guess2 > 16) {
            cout << "\nCards are only numbered from 1 to 16! Please refer to a card within that range and try again." << endl;
            continue;
        }
        else {
            validGuess2 = true;
        }
    }

    int i1 = (guess1 - 1) / 4;
    int j1 = (guess1 - 1) % 4; // Prevents out-of-bounds for disp
    int i2 = (guess2 - 1) / 4;
    int j2 = (guess2 - 1) % 4;

    if (disp[i1][j1][0].frontLetter == disp[i2][j2][0].frontLetter) { // Checks for match
        if (disp[i1][j1][0].isActive == false || disp[i2][j2][0].isActive == false) {
            cout << "\nYou've already used that card!" << endl;
            inputGuess(disp);
        }
        else {
            cout << "\nCongratulations! The letters match." << endl;
            disp[i1][j1][0].isActive = false;
            disp[i2][j2][0].isActive = false;
            unsigned int currentTime = 1; // FIXME: Placeholder, need to integrate timer
            addScore(currentScore, currentTime);
        }
    }
    else {
        cout << "\nSorry, the letters do not match." << endl;
        inputGuess(disp);
    }
}

int main() {
    string alph = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // Having this as a vector was causing errors
    vector<char> listOfLetters;
    int k = 0;
    char random, letter;

    // Backside of cards
    vector<int> allBackNums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    int counter = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            disp[i][j][0].backNumber = allBackNums[counter];
            counter++;
        }
    }
    counter = 0;

    // Getting letters for cards
    srand(time(0));
    for (int i = 0; i < 8; i++) {
        random = rand() % alph.size();
        letter = alph[random];
        // Adding to list in pairs
        listOfLetters.push_back(letter);
        listOfLetters.push_back(letter);
        alph.erase(alph.begin() + random);
    }

    // Shuffling list so it can be added to array "letters" in random order
    random_shuffle(listOfLetters.begin(), listOfLetters.end());
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            disp[i][j][0].frontLetter = listOfLetters[k];
            disp[i][j][0].isActive = true;
            k++;
        }
    }

    cout << "Thanks for playing our matching game!\n\nHow to play:\n";
    cout << "Enter a number to see what letter lies behind it. Try and match as many letters as you can!\n\n";

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout << disp[i][j][0].backNumber << " ";
        }
        cout << endl;
    }
    cout << endl;
    for (int i = 0; i < 4; i++) { // FIXME: Grid is visible for now to allow debugging. Either the matches can appear very briefly before disappearing (requires even more timer integ.) or the user can flip individual cards without guessing (requires print+update grid function)
        for (int j = 0; j < 4; j++) {
            cout << disp[i][j][0].frontLetter << " ";
        }
        cout << endl;
    }

    cout << endl;
    inputGuess(disp);

    return 0;
}
