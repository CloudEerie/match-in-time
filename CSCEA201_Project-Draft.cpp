#include <iostream>
#include <windows.h>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <chrono>
#include <unistd.h>
using namespace std;

struct Card {
    int backNumber;
    char frontLetter;
    bool isActive;
};

vector<vector<vector<Card>>> disp(4, vector<vector<Card>>(4, vector<Card>(1))); // Got very convoluted but necessary to pull from struct properly
unsigned int currentScore = 0;

// Reduced scope of project by replacing old timer function w/ using chrono "stopwatch" in inputGuess()

void addScore(unsigned int& currentScore, double timeDuration) {
    double timeBonus = ceil((25.0 / timeDuration)); // Max points is 25, Min points is 1
    currentScore += timeBonus;
}

void printGrid(vector<vector<vector<Card>>>& disp) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if ((disp[i][j][0]).isActive) {
                cout << ((disp[i][j][0].backNumber < 10) ? ("0" + to_string(disp[i][j][0].backNumber)) : to_string(disp[i][j][0].backNumber)) << " ";
            }
            else {
                cout << disp[i][j][0].frontLetter << " ";
            }
        }
        cout << endl;
    }
}

void inputGuess(vector<vector<vector<Card>>>& disp) { // FIXME: Formatting for grid w/ some answers correct looks awkward, with backNumber taking two spaces and frontLetter taking one.
    int guess1, guess2;
    bool validGuess1 = false, validGuess2 = false;
    auto startTime = chrono::high_resolution_clock::now(); // FIXME: startTime & endTime need to persist across incorrect answers and reset upon correct ones.

    while (validGuess1 == false) { // FIXME: May be worth making into separate function when cleaning up, but not a priority
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
            cout << "\nYou\'ve already used that card!" << endl;
            inputGuess(disp);
        }
        else {
            auto endTime = chrono::high_resolution_clock::now();
            auto timeDuration = static_cast<double>(chrono::duration_cast<chrono::seconds>(endTime - startTime).count());
            addScore(currentScore, timeDuration);
            cout << "\nCongratulations! The letters match." << endl;
            cout << "\nScore: " << currentScore << "\nTook " << timeDuration << (timeDuration == 1 ? " second" : " seconds") << " to match in time!" << endl;
            disp[i1][j1][0].isActive = false;
            disp[i2][j2][0].isActive = false;
            printGrid(disp);
        }
    }
    else {
        cout << "\nSorry, the letters do not match." << endl;
        inputGuess(disp);
    }
}

bool anyCardsLeft(vector<vector<vector<Card>>>& disp) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (disp[i][j][0].isActive) {
                return true;
            }
        }
    }
    return false;
}

void introDisplay(vector<vector<vector<Card>>>& disp, int instructWait = 10, int hintWait = 5) {
    cout << "Thanks for playing our matching game!\n\nHow to play:\n";
    cout << "Enter a number to see what letter lies behind it. Try and match as many letters as you can!\n\n";
    Sleep(instructWait * 1000); // 10 seconds to read instructions; ideally user would see countdown until hint so they don't get confused
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout << disp[i][j][0].frontLetter << " ";
        }
        cout << endl;
    }
    Sleep(hintWait * 1000); // 5 seconds to view answers
    system("cls"); // clear screen
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

    introDisplay(disp);

    /* for (int i = 0; i < 4; i++) { // Testbench for viewing every Card's elements beside each other. Remove /* to re-implement.
        for (int j = 0; j < 4; j++) {
            cout << disp[i][j][0].backNumber << " ";
        }
        cout << endl;
    }
    cout << endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout << disp[i][j][0].frontLetter << " ";
        }
        cout << endl;
    } */

    printGrid(disp);
    while (anyCardsLeft(disp)) { // Starts game and keeps going until finished
        inputGuess(disp);
    }
    cout << "\nFinal Score: " << currentScore << "\n\nThanks for playing!" << endl;

    return 0;
}
