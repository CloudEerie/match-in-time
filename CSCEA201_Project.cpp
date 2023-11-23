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

unsigned int currentScore = 0;
chrono::high_resolution_clock::time_point startTime;

// Reduced scope of project by replacing old timer function w/ using chrono "stopwatch" in inputGuess()

void addScore(unsigned int& currentScore, double timeDuration) {
    double timeBonus = ceil((30.0 / timeDuration)); // Max bonus is 30, Min bonus is 1
    currentScore += timeBonus + 5;
}

void printGrid(vector<vector<vector<Card>>>& disp) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if ((disp[i][j][0]).isActive) {
                cout << ((disp[i][j][0].backNumber < 10) ? ("0" + to_string(disp[i][j][0].backNumber)) : to_string(disp[i][j][0].backNumber)) << " ";
            }
            else {
                cout << " " << disp[i][j][0].frontLetter << " ";
            }
        }
        cout << endl;
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

int getValidGuess(string inputSeq = "first") {
    int guess;
    bool validGuess = false;

    while (!validGuess) {
        cout << "\nEnter the " << inputSeq << " card\'s number: ";
        cin >> guess;

        if (cin.fail() || guess < 1 || guess > 16) {
            cout << "\nInvalid input! Please enter a number between 1 and 16." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            validGuess = true;
        }
    }

    return guess;
}

void inputGuess(vector<vector<vector<Card>>>& disp, bool newTimer = true) {
    int guess1, guess2;
    if (newTimer == true) {
        startTime = chrono::high_resolution_clock::now();
    }
    guess1 = getValidGuess(), guess2 = getValidGuess("second");

    int i1 = (guess1 - 1) / 4;
    int j1 = (guess1 - 1) % 4; // Prevents out-of-bounds for disp
    int i2 = (guess2 - 1) / 4;
    int j2 = (guess2 - 1) % 4;

    if (disp[i1][j1][0].frontLetter == disp[i2][j2][0].frontLetter) { // Checks for match
        if (disp[i1][j1][0].isActive == false || disp[i2][j2][0].isActive == false) {
            cout << "\nYou\'ve already used that card!" << endl;
            inputGuess(disp, false);
        }
        else if (disp[i1][j1][0].backNumber == disp[i2][j2][0].backNumber) {
            cout << "\nYou\'ve called the same card twice!" << endl;
            inputGuess(disp, false);
        }
        else {
            auto endTime = chrono::high_resolution_clock::now();
            auto timeDuration = static_cast<double>(chrono::duration_cast<chrono::seconds>(endTime - startTime).count());
            if (timeDuration < 1) {
                timeDuration = 1;
            }
            disp[i1][j1][0].isActive = false;
            disp[i2][j2][0].isActive = false;
            addScore(currentScore, timeDuration);
            cout << "\nCongratulations! The letters match." << endl;
            cout << ((anyCardsLeft(disp)) ? "\nScore: " : "\nFinal Score: ") << currentScore << "\nTook " << timeDuration << (timeDuration == 1 ? " second" : " seconds") << " to match in time!" << endl;
            printGrid(disp);
        }
    }
    else {
        cout << "\nSorry, the letters do not match." << endl;
        inputGuess(disp, false);
    }
}

void introDisplay(vector<vector<vector<Card>>>& disp, int hintWait = 10) {
    cout << "Thanks for playing our matching game!\n\nHow to play:\n\n";
    cout << "You\'ll briefly see 16 cards that each show 8 different letters.\n\nThey\'ll then flip to show only numbers.\n\nMatch from memory as quickly as you can to earn extra points!\n\n";
    system("pause");
    cout << endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout << " " << disp[i][j][0].frontLetter << " ";
        }
        cout << endl;
    }
    Sleep(hintWait * 1000); // X seconds to view answers; 10 by default
    system("cls"); // Clears screen
}

int main() {
    vector<vector<vector<Card>>> disp(4, vector<vector<Card>>(4, vector<Card>(1))); // Got very convoluted but necessary to pull from struct properly
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

    printGrid(disp);
    while (anyCardsLeft(disp)) { // Starts game and keeps going until finished
        inputGuess(disp, true);
    }
    cout << "\nThanks for playing!" << endl;

    return 0;
}
